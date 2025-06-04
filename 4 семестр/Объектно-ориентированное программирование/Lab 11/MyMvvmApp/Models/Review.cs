using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace photoLoom.Models
{
    public class Review
    {
        public string UserLogin { get; set; }
        public string UserPhotoPath { get; set; }
        public string ReviewText { get; set; }
        public string DateReviewText { get; set; }
    }
}

