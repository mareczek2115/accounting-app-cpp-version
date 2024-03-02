const express = require('express');
const bcrypt = require('bcrypt');
const { Pracownik, Konto, Operacja, Stanowisko, Pensja } = require('./models');

const router = express.Router();

router.post('/wyplaty', async (req, res) => {
  const { employee_inner_id } = req.body;
  try {
    const data = await Pracownik.findOne({
      where: { identyfikator_firmowy_pracownika: employee_inner_id },
      include: [{ model: Stanowisko, required: true }],
    });
    res.json(data);
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/nowa-operacja', async (req, res) => {
  const { id_wydzialu, kwota, opis } = req.body;
  try {
    await Operacja.create({ id_wydzialu, kwota, opis });
    res.json({ res: 'success' });
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/nowa-wyplata', async (req, res) => {
  const { id_pracownika, kwota } = req.body;
  try {
    await Pensja.create({
      id_pracownika,
      kwota,
      data: new Date().toJSON().slice(0, 10),
    });
    res.json({ res: 'success' });
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/logowanie', async (req, res) => {
  const { login, haslo } = req.body;
  try {
    const data = await Konto.findOne({ where: { login } });
    if (!data) {
      res.json({ res: 'error' });
    } else {
      bcrypt.compare(haslo, data.getDataValue('haslo'), (err, areEqual) => {
        if (areEqual) {
          res.json({ res: 'success', konto: data.getDataValue('typ_konta') });
        } else {
          res.json({ res: 'error' });
        }
      });
    }
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/nowe-konto', async (req, res) => {
  const { login, haslo } = req.body;
  try {
    const data = await Konto.findOne({ where: { login } });
    if (!data) {
      bcrypt.hash(haslo, 6, async (err, hash) => {
        await Konto.create({ login, haslo: hash, typ_konta: 'user' });
        res.json({ res: 'success' });
      });
    } else {
      res.json({ res: 'error' });
    }
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/nowe-stanowisko', async (req, res) => {
  const { nazwa_stanowiska, pensja_bazowa } = req.body;
  console.log(nazwa_stanowiska);
  console.log(pensja_bazowa);
  try {
    await Stanowisko.create({ nazwa_stanowiska, pensja_bazowa });
    res.json({ res: 'success' });
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/nowy-pracownik', async (req, res) => {
  const { imie, nazwisko, id_stanowiska, identyfikator_firmowy_pracownika } =
    req.body;
  try {
    const data = await Pracownik.findOne({
      where: { identyfikator_firmowy_pracownika },
    });
    if (data) {
      res.json({ res: 'error' });
    } else {
      await Pracownik.create({
        imie,
        nazwisko,
        id_stanowiska,
        identyfikator_firmowy_pracownika,
      });
      res.json({ res: 'success' });
    }
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/edytuj-stanowisko', async (req, res) => {
  const { id, nazwa_stanowiska, pensja_bazowa } = req.body;
  try {
    await Stanowisko.update(
      { nazwa_stanowiska, pensja_bazowa },
      { where: { id } }
    );
    res.json({ res: 'success' });
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/edytuj-konto', async (req, res) => {
  const { id_konta, login, haslo } = req.body;
  try {
    const data = await Konto.findOne({ where: { id_konta } });
    console.log(data.getDataValue('haslo'), haslo);
    if (data.getDataValue('haslo') == haslo) {
      await Konto.update({ login }, { where: { id_konta } });
      res.json({ res: 'success' });
    } else {
      bcrypt.hash(haslo, 6, async (err, hash) => {
        if (err) throw err;
        else {
          await Konto.update({ login, haslo: hash }, { where: { id_konta } });
          res.json({ res: 'success' });
        }
      });
    }
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

router.post('/edytuj-pracownika', async (req, res) => {
  const {
    id,
    imie,
    nazwisko,
    id_stanowiska,
    identyfikator_firmowy_pracownika,
  } = req.body;
  try {
    await Pracownik.update(
      { imie, nazwisko, id_stanowiska, identyfikator_firmowy_pracownika },
      { where: { id } }
    );
    res.json({ res: 'success' });
  } catch (error) {
    console.error('Unable to connect to the database:', error);
    res.end();
  }
});

module.exports = router;
