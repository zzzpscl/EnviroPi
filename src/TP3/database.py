import mysql.connector

# Connect to server
db = mysql.connector.connect(
    host="127.0.0.1",
    port=3306,
    user="mike",
    password="s3cre3t!")

# Get a cursor
db_action = db.cursor()

# Execute a query
db_action.execute("SELECT CURDATE()")

# Fetch one result
row = db_action.fetchone()
print("Current date is: {0}".format(row[0]))

# Close connection
db_action.close()