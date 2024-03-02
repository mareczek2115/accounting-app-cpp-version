const sequelize = require('./db_conn');
const { DataTypes } = require('sequelize');

const Pracownik = sequelize.define(
  'pracownicy',
  {
    id: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      allowNull: false,
      autoIncrement: true,
    },
    imie: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    nazwisko: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    id_stanowiska: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
    identyfikator_firmowy_pracownika: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
  },
  { tableName: 'pracownicy', timestamps: false }
);

const Konto = sequelize.define(
  'konta',
  {
    id_konta: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      allowNull: false,
      autoIncrement: true,
    },
    login: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    haslo: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    typ_konta: {
      type: DataTypes.STRING,
      allowNull: false,
    },
  },
  { tableName: 'konta', timestamps: false }
);

const Operacja = sequelize.define(
  'operacje',
  {
    id_operacji: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      allowNull: false,
      autoIncrement: true,
    },
    id_wydzialu: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
    kwota: {
      type: DataTypes.DECIMAL,
      allowNull: false,
    },
    opis: {
      type: DataTypes.STRING,
      allowNull: false,
    },
  },
  { tableName: 'operacje', timestamps: false }
);

const Stanowisko = sequelize.define(
  'stanowiska',
  {
    id: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      allowNull: false,
      autoIncrement: true,
    },
    nazwa_stanowiska: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    pensja_bazowa: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
  },
  { tableName: 'stanowiska', timestamps: false }
);

const Wydzial = sequelize.define(
  'wydzialy',
  {
    id_wydzialu: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      allowNull: false,
      autoIncrement: true,
    },
    nazwa_wydzialu: {
      type: DataTypes.STRING,
      allowNull: false,
    },
  },
  { tableName: 'wydzialy', timestamps: false }
);

const Pensja = sequelize.define(
  'wyplacone_pensje',
  {
    id: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      allowNull: false,
      autoIncrement: true,
    },
    id_pracownika: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
    kwota: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
    data: {
      type: DataTypes.DATEONLY,
      allowNull: false,
    },
  },
  { tableName: 'wyplacone_pensje', timestamps: false }
);

Stanowisko.hasMany(Pracownik, { foreignKey: 'id_stanowiska' });
Pracownik.belongsTo(Stanowisko, { foreignKey: 'id_stanowiska' });

Wydzial.hasMany(Operacja, { foreignKey: 'id_wydzialu' });
Operacja.belongsTo(Wydzial, { foreignKey: 'id_wydzialu' });

Pracownik.hasMany(Pensja, { foreignKey: 'id_pracownika' });
Pensja.belongsTo(Pracownik, { foreignKey: 'id_pracownika' });

module.exports = {
  Pracownik: Pracownik,
  Konto: Konto,
  Operacja: Operacja,
  Wydzial: Wydzial,
  Stanowisko: Stanowisko,
  Pensja: Pensja,
};
