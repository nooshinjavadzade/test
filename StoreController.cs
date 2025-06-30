using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Store.Data;
using Store.Models;
namespace Store.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class StoreController : ControllerBase
    {
        public class OrdersController : ControllerBase
        {
            private readonly MyDbcontext _context;

            public OrdersController(MyDbcontext context)
            {
                _context = context;
            }

            [HttpGet("allorders")]
            public ActionResult<IEnumerable<Orders>> GetOrders()
            {
                return Ok(_context.Orders
                    .Include(o => o.Customer)
                    .Include(o => o.Product)
                    .ToList());
            }

            [HttpPost("addorder")]
            public ActionResult<Orders> PlaceOrder(Orders order)
            {
                using var transaction = _context.Database.BeginTransaction();

                try
                {
                    var customer = _context.Customers.Find(order.CustomerId);
                    var product = _context.Products.Find(order.ProductId);

                    if (customer == null || product == null)
                        return BadRequest("Invalid CustomerId or ProductId");

                    _context.Orders.Add(order);
                    _context.SaveChanges();

                    transaction.Commit();

                    return Ok($"Order:{order}");
                }
                catch (Exception)
                {
                    transaction.Rollback();
                    return BadRequest("Something went wrong!");
                }
            }

            [HttpGet("showcustomers")]
            public ActionResult<IEnumerable<Customer>> GetAllCustomers()
            {
                var customers = _context.Customers.ToList();
                return Ok(customers);
            }

            [HttpGet("showproducts")]
            public ActionResult<IEnumerable<Product>> GetAllProducts()
            {
                var products = _context.Products.ToList();
                return Ok(products);
            }

            [HttpPost("addcustomers")]
            public ActionResult<Customer> AddCustomer(Customer customer)
            {
                _context.Customers.Add(customer);
                _context.SaveChanges();

                return Ok($"Customer ID:{customer.Id}");
            }

            [HttpPost("addproducts")]
            public ActionResult<Product> AddProduct(Product product)
            {
                _context.Products.Add(product);
                _context.SaveChanges();

                return Ok($"Your product ID:{product.Id}");


            }
        }
    }
}

