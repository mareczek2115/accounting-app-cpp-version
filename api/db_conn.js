const { Sequelize } = require('sequelize');

const sequelize = new Sequelize('projekt', 'root', '', {
  host: 'localhost',
  dialect: 'mysql',
});

module.exports = sequelize;
