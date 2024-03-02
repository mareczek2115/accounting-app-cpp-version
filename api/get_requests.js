const express = require('express');
const {
  Pracownik,
  Konto,
  Operacja,
  Wydzial,
  Stanowisko,
  Pensja,
} = require('./models');

const router = express.Router();

router.get('/pracownicy', async (_, res) => {
  try {
    const data = await Pracownik.findAll({
      include: [{ required: true, model: Stanowisko }],
    });
    res.json(data);
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.get('/operacje', async (_, res) => {
  try {
    const data = await Operacja.findAll({
      include: [{ required: true, model: Wydzial }],
    });
    res.json(data);
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.get('/wydzialy', async (_, res) => {
  try {
    const data = await Wydzial.findAll();
    res.json(data);
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.get('/stanowiska', async (req, res) => {
  try {
    const data = await Stanowisko.findAll();
    res.json(data);
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.get('/konta', async (req, res) => {
  try {
    const data = await Konto.findAll();
    res.json(data);
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.get('/wyplaty', async (req, res) => {
  try {
    const data = await Pensja.findAll({
      include: [{ required: true, model: Pracownik }],
    });
    res.json(data);
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

module.exports = router;
