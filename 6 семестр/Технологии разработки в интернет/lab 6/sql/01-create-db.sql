IF DB_ID(N'Celebrities') IS NULL
BEGIN
  CREATE DATABASE [Celebrities];
END;
GO

USE [Celebrities];
GO

IF OBJECT_ID(N'dbo.Celebrities', N'U') IS NULL
BEGIN
  CREATE TABLE [dbo].[Celebrities](
    [Id] [int] IDENTITY(1,1) NOT NULL,
    [FullName] [nvarchar](50) NOT NULL,
    [Nationality] [nvarchar](2) NOT NULL,
    [ReqPhotoPath] [nvarchar](200) NULL,
    CONSTRAINT [PK_Celebrities] PRIMARY KEY CLUSTERED ([Id] ASC)
  );
END;
GO
