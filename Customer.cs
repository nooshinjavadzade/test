using System.ComponentModel.DataAnnotations;
namespace Store.Models

{
    public class Customer
    {
        [Key]
        public int Id { get; set; }
        [Required]
        public string Name { get; set; }
        public string Address { get; set; }
        public string Phone { get; set; }
        public ICollection<Orders>? Orders { get; set; } 
    }
}
