using Microsoft.EntityFrameworkCore;
using Store.Models;
namespace Store.Data
{
    public class MyDbcontext:DbContext
    {
        public MyDbcontext(DbContextOptions<MyDbcontext> options) : base(options) { }

        public DbSet<Product> Products { get; set; }
        public DbSet<Customer> Customers { get; set; }
        public DbSet<Orders> Orders { get; set; }
    }
}
