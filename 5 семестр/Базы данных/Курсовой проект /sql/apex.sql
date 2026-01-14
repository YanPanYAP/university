ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;

-- Установка IMAGE_PREFIX
BEGIN
    apex_instance_admin.set_parameter(
        p_parameter => 'IMAGE_PREFIX',
        p_value     => 'https://static.oracle.com/cdn/apex/24.2.0/'
    );
END;
/
COMMIT;

-- Все instance параметры APEX
SELECT *
FROM apex_instance_parameters
ORDER BY name;

-- Таймаут простоя сессии
BEGIN
  APEX_INSTANCE_ADMIN.SET_PARAMETER(
    p_parameter => 'MAX_SESSION_IDLE_SEC',
    p_value     => 28800
  );
END;
/
COMMIT;

-- Максимальная длина сессии
BEGIN
  APEX_INSTANCE_ADMIN.SET_PARAMETER(
    p_parameter => 'MAX_SESSION_LENGTH_SEC',
    p_value     => 43200
  );
END;
/
COMMIT;

-- Проверка таймаутов
SELECT name, value
FROM apex_instance_parameters
WHERE name IN (
  'MAX_SESSION_IDLE_SEC',
  'MAX_SESSION_LENGTH_SEC'
);

-- Создание schema и tablespace под Workspace
DECLARE
  l_ts_name     VARCHAR2(30) := 'CRS_TS';
  l_db_username VARCHAR2(30) := 'CRS_CUR';
  l_db_password VARCHAR2(30) := 'MyPassword123';
  l_datafile    VARCHAR2(100) := '/opt/oracle/oradata/ORCLCDB/CRS_TS01.dbf';
BEGIN
  EXECUTE IMMEDIATE
    'create tablespace ' || l_ts_name ||
    ' datafile ''' || l_datafile || ''' size 100m autoextend on next 100m';

  EXECUTE IMMEDIATE
    'create user ' || l_db_username ||
    ' identified by ' || l_db_password ||
    ' default tablespace ' || l_ts_name ||
    ' quota unlimited on ' || l_ts_name;

  FOR cur_rec IN (
    SELECT privilege
    FROM dba_sys_privs
    WHERE grantee = 'APEX_GRANTS_FOR_NEW_USERS_ROLE'
  )
  LOOP
    EXECUTE IMMEDIATE 'grant ' || cur_rec.privilege || ' to ' || l_db_username;
  END LOOP;
END;
/


-- Создание Workspace и администратора
DECLARE
  l_workspace       VARCHAR2(30) := 'COURSE_WORKSPACE';
  l_db_username     VARCHAR2(30) := 'CRS_CUR';
  l_admin_username  VARCHAR2(30) := 'admin';
  l_admin_password  VARCHAR2(30) := 'MyPassword123';
  l_admin_email     VARCHAR2(50) := 'yan.paulovich@gmail.com';
BEGIN
  apex_instance_admin.add_workspace(
      p_workspace      => l_workspace,
      p_primary_schema => l_db_username
  );

  apex_util.set_workspace(p_workspace => l_workspace);

  apex_util.create_user(
      p_user_name                    => l_admin_username,
      p_web_password                 => l_admin_password,
      p_developer_privs              => 'ADMIN:CREATE:DATA_LOADER:EDIT:HELP:MONITOR:SQL',
      p_email_address                => l_admin_email,
      p_default_schema               => l_db_username,
      p_change_password_on_first_use => 'N'
  );

  COMMIT;
END;
/


-- Проверка Workspace
SELECT workspace_name, schema
FROM apex_workspace_schemas
WHERE workspace_name = 'COURSE_WORKSPACE';

-- Проверка пользователей Workspace
SELECT workspace_name, user_name
FROM apex_workspace_apex_users
WHERE workspace_name = 'COURSE_WORKSPACE';

-- Скрипт администратора APEX
@/path/to/apex/admin/apex_admin.sql

BEGIN
  apex_util.set_workspace(p_workspace => 'COURSE_WORKSPACE');

  -- GLAV_ADMIN
  apex_util.create_user(
      p_user_name      => 'apex_glav_admin',
      p_web_password   => 'Glav123',
      p_developer_privs => 'ADMIN:CREATE:EDIT:HELP:MONITOR:SQL',
      p_default_schema => 'CRS_CUR',
      p_change_password_on_first_use => 'N'
  );

  -- ADMIN
  apex_util.create_user(
      p_user_name      => 'apex_admin',
      p_web_password   => 'Admin123',
      p_developer_privs => 'CREATE:EDIT:HELP',
      p_default_schema => 'CRS_CUR',
      p_change_password_on_first_use => 'N'
  );

  -- USER
  apex_util.create_user(
      p_user_name      => 'apex_user',
      p_web_password   => 'User123',
      p_developer_privs => 'EDIT',
      p_default_schema => 'CRS_CUR',
      p_change_password_on_first_use => 'N'
  );

  COMMIT;
END;
/


