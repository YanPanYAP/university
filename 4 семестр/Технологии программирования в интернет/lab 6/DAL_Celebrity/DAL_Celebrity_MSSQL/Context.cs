﻿using Microsoft.EntityFrameworkCore;

namespace DAL_Celebrity_MSSQL
{
    public class Context : DbContext   
    {
        public string? ConnectionString { get; private set; } = null;
        public Context(string connextionString) : base()
        {
            ConnectionString = connextionString;
        }

        public Context() : base()
        {

        }

        public DbSet<Celebrity> Celebrities { get; set; }

        public DbSet<LifeEvent> LifeEvents { get; set; }

        
        // для настройки параметров подключения к базе данных.
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {

               
            if (this.ConnectionString is null)
            {
                this.ConnectionString = "Server=192.168.1.111,1433; Database=Lab6_Db; User Id=sa; Password=YourStrong@Passw0rd; TrustServerCertificate=True";
            }

            optionsBuilder.UseSqlServer(this.ConnectionString);
        }


        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Celebrity>().ToTable("Celebrities").HasKey(p => p.Id);   
            modelBuilder.Entity<Celebrity>().Property(p => p.Id).IsRequired();        
            modelBuilder.Entity<Celebrity>().Property(p => p.FullName).IsRequired().HasMaxLength(50);      
            modelBuilder.Entity<Celebrity>().Property(p => p.Nationality).IsRequired();
            modelBuilder.Entity<Celebrity>().Property(p => p.ReqPhotoPath).HasMaxLength(200);

            modelBuilder.Entity<LifeEvent>().ToTable("LifeEvents").HasKey(p => p.Id);              
            modelBuilder.Entity<LifeEvent>().ToTable("LifeEvents");
            modelBuilder.Entity<LifeEvent>().Property(p => p.Id).IsRequired();          
            modelBuilder.Entity<LifeEvent>().ToTable("LifeEvents").HasOne<Celebrity>().WithMany().HasForeignKey(p => p.CelebrityId);  // Указываем, что LifeEvent связан с Celebrity (один)   // ... и у Celebrity может быть много LifeEvent  // Связь через внешний ключ CelebrityId      
            modelBuilder.Entity<LifeEvent>().Property(p => p.CelebrityId).IsRequired();
            modelBuilder.Entity<LifeEvent>().Property(p => p.Date);             
            modelBuilder.Entity<LifeEvent>().Property(p => p.Description).HasMaxLength(256);
            modelBuilder.Entity<LifeEvent>().Property(p => p.ReqPhotoPath).HasMaxLength(256);


            base.OnModelCreating(modelBuilder);
        }

    }
}
