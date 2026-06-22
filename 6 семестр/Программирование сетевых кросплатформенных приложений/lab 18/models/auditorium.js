const { DataTypes } = require('sequelize');
const sequelize = require('../db');

module.exports = sequelize.define('AUDITORIUM', {
    AUDITORIUM: { type: DataTypes.STRING(20), primaryKey: true },
    AUDITORIUM_TYPE: DataTypes.STRING(10),
    AUDITORIUM_CAPACITY: DataTypes.INTEGER,
    AUDITORIUM_NAME: DataTypes.STRING(50)
}, {
    tableName: 'AUDITORIUM',
    timestamps: false
});