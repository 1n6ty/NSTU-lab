using System;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;
using System.Collections.Generic;

namespace WarehouseWinApp
{
    public static class DatabaseHelper
    {
        private static string connectionString = 
            ConfigurationManager.ConnectionStrings["WarehouseDB"].ConnectionString;

        // Выполнить запрос без возврата данных
        public static int ExecuteNonQuery(string query, params SqlParameter[] parameters)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                using (SqlCommand cmd = new SqlCommand(query, conn))
                {
                    if (parameters != null)
                        cmd.Parameters.AddRange(parameters);
                    
                    conn.Open();
                    return cmd.ExecuteNonQuery();
                }
            }
        }

        // Получить таблицу данных
        public static DataTable GetDataTable(string query, params SqlParameter[] parameters)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                using (SqlCommand cmd = new SqlCommand(query, conn))
                {
                    if (parameters != null)
                        cmd.Parameters.AddRange(parameters);
                    
                    using (SqlDataAdapter da = new SqlDataAdapter(cmd))
                    {
                        DataTable dt = new DataTable();
                        da.Fill(dt);
                        return dt;
                    }
                }
            }
        }

        // Получить одно значение
        public static object GetScalar(string query, params SqlParameter[] parameters)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                using (SqlCommand cmd = new SqlCommand(query, conn))
                {
                    if (parameters != null)
                        cmd.Parameters.AddRange(parameters);
                    
                    conn.Open();
                    return cmd.ExecuteScalar();
                }
            }
        }

        // Получить список товаров
        public static List<Product> GetProducts()
        {
            List<Product> products = new List<Product>();
            string query = "SELECT * FROM Products ORDER BY Name";
            DataTable dt = GetDataTable(query);

            foreach (DataRow row in dt.Rows)
            {
                products.Add(new Product
                {
                    Id = Convert.ToInt32(row["Id"]),
                    Name = row["Name"].ToString(),
                    Article = row["Article"].ToString(),
                    Category = row["Category"].ToString(),
                    Unit = row["Unit"].ToString(),
                    Price = Convert.ToDecimal(row["Price"])
                });
            }
            return products;
        }
    }
}