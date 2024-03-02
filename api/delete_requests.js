const express = require('express');
const { Pracownik, Konto, Stanowisko, Pensja } = require('./models');

const router = express.Router();

router.delete('/usun-stanowisko', async (req, res) => {
  const { id } = req.body;
  console.log(id);
  try {
    const data = await Pracownik.findAll({ where: { id_stanowiska: id } });
    if (data.length > 0) {
      res.json({ res: 'error' });
    } else {
      await Stanowisko.destroy({ where: { id } });
      res.json({ res: 'success' });
    }
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.delete('/usun-konto', async (req, res) => {
  const { id_konta } = req.body;
  try {
    await Konto.destroy({ where: { id_konta } });
    res.json({ res: 'success' });
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.delete('/usun-pracownika', async (req, res) => {
  const { id } = req.body;
  try {
    await Pensja.destroy({ where: { id_pracownika: id } });
    await Pracownik.destroy({ where: { id } });
    res.json({ res: 'success' });
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

module.exports = router;
