﻿using System.ComponentModel.DataAnnotations;

namespace Store.Models
{
    public class Product
    {
        [Key]
        public int Id { get; set; }
        [Required]
        public string Name { get; set; } 
        [Required]
        public int Price { get; set; }
    }
}
