USE [Celebrities];
GO

INSERT INTO dbo.Celebrities (FullName, Nationality, ReqPhotoPath)
VALUES
  (N'Albert Einstein', N'DE', N'/photos/einstein.jpg'),
  (N'Audrey Hepburn', N'GB', N'/photos/hepburn.jpg');
GO

UPDATE dbo.Celebrities
SET ReqPhotoPath = N'/photos/albert-einstein.jpg'
WHERE FullName = N'Albert Einstein';
GO

DELETE FROM dbo.Celebrities
WHERE FullName = N'Audrey Hepburn';
GO

SELECT Id, FullName, Nationality, ReqPhotoPath
FROM dbo.Celebrities
ORDER BY Id;
GO
