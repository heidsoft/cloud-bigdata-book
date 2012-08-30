MYSQL * db_connect(const char *dbname)
 
{
 
  MYSQL *db = mysql_init(NULL);
 
  if (!db)
 
    die(db, "mysql_init failed: no memory");
 
  /*
 
   * Notice that the client and server use separate group names.
 
   * This is critical, because the server does not accept the
 
   * client's options, and vice versa.
 
   */
 
  mysql_options(db, MYSQL_READ_DEFAULT_GROUP, "test2_libmysqld_CLIENT");
 
  if (!mysql_real_connect(db, NULL, NULL, NULL, dbname, 0, NULL, 0))
 
    die(db, "mysql_real_connect failed: %s", mysql_error(db));
 
 
 
  return db;
 
}


static void
 
die(MYSQL *db, char *fmt, ...)
 
{
 
  va_list ap;
 
  va_start(ap, fmt);
 
  vfprintf(stderr, fmt, ap);
 
  va_end(ap);
 
  (void)putc('/n', stderr);
 
  if (db)
 
    db_disconnect(db);
 
  exit(EXIT_FAILURE);
 
}


MYSQL *
 
db_connect(const char *dbname)
 
{
 
  MYSQL *db = mysql_init(NULL);
 
  if (!db)
 
    die(db, "mysql_init failed: no memory");
 
  /*
 
   * Notice that the client and server use separate group names.
 
   * This is critical, because the server does not accept the
 
   * client's options, and vice versa.
 
   */
 
  mysql_options(db, MYSQL_READ_DEFAULT_GROUP, "test2_libmysqld_CLIENT");
 
  if (!mysql_real_connect(db, NULL, NULL, NULL, dbname, 0, NULL, 0))
 
    die(db, "mysql_real_connect failed: %s", mysql_error(db));
 
 
 
  return db;
 
}


void
 
db_do_query(MYSQL *db, const char *query)
 
{
 
  if (mysql_query(db, query) != 0)
 
    goto err;
 
 
 
  if (mysql_field_count(db) > 0)
 
  {
 
    MYSQL_RES   *res;
 
    MYSQL_ROW    row, end_row;
 
    int num_fields;
 
 
 
    if (!(res = mysql_store_result(db)))
 
     goto err;
 
    num_fields = mysql_num_fields(res);
 
    while ((row = mysql_fetch_row(res)))
 
    {
 
     (void)fputs(">> ", stdout);
 
      for (end_row = row + num_fields; row < end_row; ++row)
 
        (void)printf("%s/t", row ? (char*)*row : "NULL");
 
      (void)fputc('/n', stdout);
 
    }
 
    (void)fputc('/n', stdout);
 
    mysql_free_result(res);
 
  }
 
  else
 
    (void)printf("Affected rows: %lld/n", mysql_affected_rows(db));
 
 
 
  return;
 
 
 
err:
 
  die(db, "db_do_query failed: %s [%s]", mysql_error(db), query);
 
}