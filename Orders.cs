﻿using System.ComponentModel.DataAnnotations;
namespace Store.Models
{
    public class Orders
    {
        [Key]
        public int Id { get; set; }
        public int CustomerId { get; set; }
        public Customer? Customer { get; set; }

        public int ProductId { get; set; }
        public Product? Product { get; set; }

    }
}
