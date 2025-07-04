#!/bin/bash

echo "Configuring password for mysql. This will save the credentials as encrypted data."

# Check if login-path exists
if ! mysql_config_editor print --all | grep -q "^\[$LOGIN_PATH\]"; then
    echo "Setting mysql_config_editor login-path: $LOGIN_PATH"
    mysql_config_editor set --login-path="$LOGIN_PATH" --user="$USER" --password
else
    echo "Login-path '$LOGIN_PATH' already exists. Skipping setup."
fi

DB_NAME="unit_test_db"

# Create database and insert schema/data
sudo mysql --login-path=local <<EOF
DROP DATABASE IF EXISTS $DB_NAME;
CREATE DATABASE $DB_NAME;
USE $DB_NAME;

CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    age INT
);

INSERT INTO users (username, email, age) VALUES
('alice', 'alice@example.com', 30),
('bob', 'bob@example.com', 25),
('charlie', 'charlie@example.com', 35);
EOF

echo "MySQL database '$DB_NAME' created with sample data."

sudo mysql -e "SHOW DATABASES;"
sudo mysql -D $DB_NAME -e "SHOW TABLES;"
sudo mysql -D $DB_NAME -e "SELECT * FROM users;"
