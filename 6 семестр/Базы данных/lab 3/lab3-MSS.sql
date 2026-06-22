USE TaskControlDB;

USE TaskControlDB;

UPDATE dbo.Department SET parent_department_id = NULL WHERE department_id = 1;
UPDATE dbo.Department SET parent_department_id = 1 WHERE department_id IN (2, 3, 4, 13, 14, 15);
UPDATE dbo.Department SET parent_department_id = 2 WHERE department_id IN (5, 6, 11, 12);
UPDATE dbo.Department SET parent_department_id = 3 WHERE department_id IN (7, 8);
UPDATE dbo.Department SET parent_department_id = 4 WHERE department_id IN (9, 10);


-- 1
IF COL_LENGTH('dbo.Department', 'department_node') IS NULL
BEGIN
    ALTER TABLE dbo.Department
    ADD department_node HIERARCHYID NULL;
END;

IF EXISTS (
    SELECT 1
    FROM sys.indexes
    WHERE name = 'UX_Department_DepartmentNode'
      AND object_id = OBJECT_ID('dbo.Department')
)
BEGIN
    DROP INDEX UX_Department_DepartmentNode ON dbo.Department;
END;

IF EXISTS (
    SELECT 1
    FROM sys.columns
    WHERE object_id = OBJECT_ID('dbo.Department')
      AND name = 'department_node'
      AND is_nullable = 0
)
BEGIN
    ALTER TABLE dbo.Department
    ALTER COLUMN department_node HIERARCHYID NULL;
END;

UPDATE dbo.Department
SET department_node = NULL;

DECLARE @currentDepartmentId INT;
DECLARE @currentParentId INT;
DECLARE @parentNode HIERARCHYID;
DECLARE @lastChild HIERARCHYID;
DECLARE @newNode HIERARCHYID;

UPDATE dbo.Department
SET department_node = hierarchyid::GetRoot()
WHERE parent_department_id IS NULL;

WHILE EXISTS (
    SELECT 1
    FROM dbo.Department d
    WHERE d.department_node IS NULL
      AND EXISTS (
          SELECT 1
          FROM dbo.Department p
          WHERE p.department_id = d.parent_department_id
            AND p.department_node IS NOT NULL
      )
)
BEGIN
    SELECT TOP (1)
        @currentDepartmentId = d.department_id,
        @currentParentId = d.parent_department_id
    FROM dbo.Department d
    JOIN dbo.Department p
        ON p.department_id = d.parent_department_id
    WHERE d.department_node IS NULL
      AND p.department_node IS NOT NULL
    ORDER BY d.parent_department_id, d.department_id;

    SELECT @parentNode = department_node
    FROM dbo.Department
    WHERE department_id = @currentParentId;

    SELECT @lastChild = MAX(department_node)
    FROM dbo.Department
    WHERE parent_department_id = @currentParentId;

    SET @newNode = @parentNode.GetDescendant(@lastChild, NULL);

    UPDATE dbo.Department
    SET department_node = @newNode
    WHERE department_id = @currentDepartmentId;
END;

IF EXISTS (
    SELECT 1
    FROM dbo.Department
    WHERE department_node IS NULL
)
BEGIN
    THROW 50010, N'Не удалось заполнить hierarchyid для всех подразделений.', 1;
END;

IF EXISTS (
    SELECT 1
    FROM sys.columns
    WHERE object_id = OBJECT_ID('dbo.Department')
      AND name = 'department_node'
      AND is_nullable = 1
)
BEGIN
    ALTER TABLE dbo.Department
    ALTER COLUMN department_node HIERARCHYID NOT NULL;
END;

IF EXISTS (
    SELECT 1
    FROM sys.indexes
    WHERE name = 'UX_Department_DepartmentNode'
      AND object_id = OBJECT_ID('dbo.Department')
)
BEGIN
    DROP INDEX UX_Department_DepartmentNode ON dbo.Department;
END;

CREATE UNIQUE INDEX UX_Department_DepartmentNode
ON dbo.Department(department_node);

-- 2
CREATE OR ALTER PROCEDURE dbo.sp_GetSubDepartments
    @parent_id INT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @parentNode HIERARCHYID;
    DECLARE @parentLevel INT;

    SELECT
        @parentNode = department_node,
        @parentLevel = department_node.GetLevel()
    FROM dbo.Department
    WHERE department_id = @parent_id;

    IF @parentNode IS NULL
    BEGIN
        THROW 50001, N'Указанный родительский узел не найден.', 1;
    END;

    SELECT
        d.department_id,
        d.department_name,
        d.parent_department_id,
        d.department_node.ToString() AS hierarchy_path,
        d.department_node.GetLevel() AS hierarchy_level,
        d.department_node.GetLevel() - @parentLevel AS relative_level
    FROM dbo.Department d
    WHERE d.department_node.IsDescendantOf(@parentNode) = 1
      AND d.department_id <> @parent_id
    ORDER BY d.department_node;
END;

-- 3
CREATE OR ALTER PROCEDURE dbo.sp_AddSubDepartment
    @parent_id INT,
    @name NVARCHAR(200)
AS
BEGIN
    SET NOCOUNT ON;
    SET XACT_ABORT ON;

    DECLARE @parentNode HIERARCHYID;
    DECLARE @lastChild HIERARCHYID;
    DECLARE @newNode HIERARCHYID;

    BEGIN TRANSACTION;

    SELECT @parentNode = department_node
    FROM dbo.Department WITH (UPDLOCK, HOLDLOCK)
    WHERE department_id = @parent_id;

    IF @parentNode IS NULL
    BEGIN
        ROLLBACK TRANSACTION;
        THROW 50002, N'Родительский узел для добавления не найден.', 1;
    END;

    SELECT @lastChild = MAX(department_node)
    FROM dbo.Department WITH (UPDLOCK, HOLDLOCK)
    WHERE parent_department_id = @parent_id;

    SET @newNode = @parentNode.GetDescendant(@lastChild, NULL);

    INSERT INTO dbo.Department (department_name, parent_department_id, department_node)
    VALUES (@name, @parent_id, @newNode);

    COMMIT TRANSACTION;
END;

-- 4
CREATE OR ALTER PROCEDURE dbo.sp_MoveSubDepartments
    @old_parent INT,
    @new_parent INT
AS
BEGIN
    SET NOCOUNT ON;
    SET XACT_ABORT ON;

    DECLARE @oldParentNode HIERARCHYID;
    DECLARE @newParentNode HIERARCHYID;
    DECLARE @childId INT;
    DECLARE @childNode HIERARCHYID;
    DECLARE @lastChild HIERARCHYID;
    DECLARE @newChildRoot HIERARCHYID;

    BEGIN TRANSACTION;

    SELECT @oldParentNode = department_node
    FROM dbo.Department WITH (UPDLOCK, HOLDLOCK)
    WHERE department_id = @old_parent;

    SELECT @newParentNode = department_node
    FROM dbo.Department WITH (UPDLOCK, HOLDLOCK)
    WHERE department_id = @new_parent;

    IF @oldParentNode IS NULL
    BEGIN
        ROLLBACK TRANSACTION;
        THROW 50003, N'Исходный родительский узел не найден.', 1;
    END;

    IF @newParentNode IS NULL
    BEGIN
        ROLLBACK TRANSACTION;
        THROW 50004, N'Новый родительский узел не найден.', 1;
    END;

    IF @old_parent = @new_parent
    BEGIN
        ROLLBACK TRANSACTION;
        THROW 50005, N'Исходный и новый родитель совпадают.', 1;
    END;

    IF @newParentNode.IsDescendantOf(@oldParentNode) = 1
    BEGIN
        ROLLBACK TRANSACTION;
        THROW 50006, N'Нельзя переносить подчиненных в собственную ветвь.', 1;
    END;

    DECLARE child_cursor CURSOR LOCAL FAST_FORWARD FOR
        SELECT department_id, department_node
        FROM dbo.Department
        WHERE parent_department_id = @old_parent
        ORDER BY department_node, department_id;

    OPEN child_cursor;

    FETCH NEXT FROM child_cursor INTO @childId, @childNode;

    WHILE @@FETCH_STATUS = 0
    BEGIN
        SELECT @lastChild = MAX(department_node)
        FROM dbo.Department WITH (UPDLOCK, HOLDLOCK)
        WHERE parent_department_id = @new_parent;

        SET @newChildRoot = @newParentNode.GetDescendant(@lastChild, NULL);

        UPDATE dbo.Department
        SET department_node = department_node.GetReparentedValue(@childNode, @newChildRoot)
        WHERE department_node.IsDescendantOf(@childNode) = 1;

        UPDATE dbo.Department
        SET parent_department_id = @new_parent
        WHERE department_id = @childId;

        FETCH NEXT FROM child_cursor INTO @childId, @childNode;
    END;

    CLOSE child_cursor;
    DEALLOCATE child_cursor;

    COMMIT TRANSACTION;
END;

EXEC dbo.sp_GetSubDepartments 1;
EXEC dbo.sp_GetSubDepartments 2;
EXEC dbo.sp_GetSubDepartments 3;

EXEC dbo.sp_AddSubDepartment 7, N'TEAM';

SELECT
    department_id,
    department_name,
    parent_department_id,
    department_node.ToString() AS hierarchy_path,
    department_node.GetLevel() AS hierarchy_level
FROM dbo.Department
WHERE department_name = N'TEAM';

EXEC dbo.sp_MoveSubDepartments 2, 4;

SELECT
    department_id,
    department_name,
    parent_department_id
FROM dbo.Department
WHERE parent_department_id = 3;

SELECT
    department_id,
    department_name,
    parent_department_id,
    department_node.ToString() AS hierarchy_path
FROM dbo.Department
WHERE parent_department_id = 13
ORDER BY department_node;

SELECT
    department_id,
    department_name,
    parent_department_id,
    department_node.ToString() AS hierarchy_path,
    department_node.GetLevel() AS hierarchy_level
FROM dbo.Department
ORDER BY department_node;

SELECT
    department_node.ToString() AS hierarchy_path,
    COUNT(*) AS duplicate_count
FROM dbo.Department
GROUP BY department_node
HAVING COUNT(*) > 1;
