# Accounting app

A simple accounting application that allows you to track, create and edit data related to finances - paying salaries, adding new expenses. Two types of accounts, administrator and regular user, allow you to separate permissions and access to application functions.

# Used technologies

- nana - GUI
- cpr – network queries in the application
- nlohmann-json - data conversion to JSON format
- node.js/express - REST API server
- sequelize - ORM module
- mysql - database

# Usage

In order to test the app, it is required to import the SQL file contained in the repository to the SQL server. Then, in the Node environment, run the main file. Then you can run the exe file, which should remain in the exe directory along with the dll files.

Admin test data:

- login: admin
- password: admin

User test data:

- login: user
- password: user
