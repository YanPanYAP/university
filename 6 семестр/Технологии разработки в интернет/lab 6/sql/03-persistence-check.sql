USE [Celebrities];
GO

SELECT COUNT(*) AS TotalRows FROM dbo.Celebrities;
GO

SELECT TOP (5) Id, FullName, Nationality, ReqPhotoPath
FROM dbo.Celebrities
ORDER BY Id;
GO
