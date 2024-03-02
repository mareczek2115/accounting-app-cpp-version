-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Czas generowania: 01 Mar 2024, 19:31
-- Wersja serwera: 10.4.24-MariaDB
-- Wersja PHP: 8.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Baza danych: `projekt`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `konta`
--

CREATE TABLE `konta` (
  `id_konta` int(11) NOT NULL,
  `login` varchar(255) COLLATE utf8_polish_ci NOT NULL,
  `haslo` varchar(255) COLLATE utf8_polish_ci NOT NULL,
  `typ_konta` varchar(255) COLLATE utf8_polish_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Zrzut danych tabeli `konta`
--

INSERT INTO `konta` (`id_konta`, `login`, `haslo`, `typ_konta`) VALUES
(3, 'admin', '$2b$06$pv6c5hTS17NOZLG8GwmRTe3NHxkxuuW434vvcN3jGEmBDQeRJt3WO', 'admin'),
(22, 'user', '$2b$06$Xi0hbajTJsdRfq5W74JsKe96hlScnq70VRJEjeC1Tb58B.mG8Hvc.', 'user');

--
-- Wyzwalacze `konta`
--
DELIMITER $$
CREATE TRIGGER `konta_AFTER_INSERT` AFTER INSERT ON `konta` FOR EACH ROW BEGIN
	INSERT INTO `projekt`.`konta_backup` (`id_konta`, `login`, `haslo`, `typ_konta`) VALUES(NEW.`id_konta`, NEW.`login`, NEW.`haslo`, NEW.`typ_konta`);
END
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `konta_BEFORE_INSERT` BEFORE INSERT ON `konta` FOR EACH ROW BEGIN

IF NEW.`id_konta` IS NULL OR NEW.`login` IS NULL OR TRIM(NEW.`login`) = "" OR NEW.`haslo` IS NULL OR TRIM(NEW.`haslo`) = "" OR NEW.`typ_konta` IS NULL OR TRIM(NEW.`typ_konta`) = ""
	THEN
		SIGNAL sqlstate '45000'
			SET MESSAGE_TEXT = "Missing data";
	END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `konta_backup`
--

CREATE TABLE `konta_backup` (
  `id_konta` int(11) NOT NULL,
  `login` varchar(255) COLLATE utf8_polish_ci NOT NULL,
  `haslo` varchar(255) COLLATE utf8_polish_ci NOT NULL,
  `typ_konta` varchar(255) COLLATE utf8_polish_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Zrzut danych tabeli `konta_backup`
--

INSERT INTO `konta_backup` (`id_konta`, `login`, `haslo`, `typ_konta`) VALUES
(6, 'monke', '$2b$06$xMnB9wYw3E9XIFCfVPlnfeSGukDAH3yDDx.2kzd8jODEZ4jLzCDJm', 'user'),
(7, 'zzz', '$2b$06$M9hnndbr00jTYAIVoSG.U.779mx2bEakHeO6/jbizfkf0/dAz0RMK', 'user'),
(8, 'lol', '$2b$06$3xqZApCwuK9nT2KC6eT30uaGl1KU.v92yot4XpSQOviO5h..wnsje', 'user'),
(9, 'tyga', '$2b$06$RCvbP0WdXqqosTiFJio1IuzSWF/v0Sx1bBb5oCgtFjUB8csbTUdzS', 'user'),
(10, 'xxx', '$2b$06$fTRo24vMvrbEBks8iyc2v.RAB4YGHlqgXXdVgACXSKq4JLLC5JNZy', 'user'),
(11, 'vvv', '$2b$06$v8TTprguxHjuo.VLOExUNe6QQp2wgrJbrJVhWZQqqxetl6X1oXSTO', 'user'),
(12, 'qqq', '$2b$06$5fLfD.lgc/e23FD/bgYh.Otsmp3dkNOne/pGmjV.n5.AXrgLwxsuG', 'user'),
(13, 'uuu', '$2b$06$SV4OnqZOjPOSUFyp5zSTw.FDzuPYyA7P14iqjpK3V2H4PxuO6YwzW', 'user'),
(14, 'lll', '$2b$06$AYfPowp4hFOnVsAE4RsPBekqvPJy7qGGE6t6vc.VilmY/7szayfIi', 'user'),
(15, 'bla', '$2b$06$xEXw61K6AWskVf.0gchZN.f8DBLD3cIyN1pcEOnhXxEAe4EhQ2lpK', 'user'),
(16, 'asd', '$2b$06$Hx3KfUbodiYL0px4THrCgeYfIePGHzKIHD.rNv2ulUU1/130gHuOq', 'user'),
(17, 'qwer', '$2b$06$kVIs1RNdVFOdWW4KrptAEOnhaT/MMtc8luKmJ.kphuYnRVko8fpPS', 'user'),
(18, 'zxc', '$2b$06$L/DrFHMeWfDrgS9AHAjInefIpPGrW4YEQbAoEOfOck0WOAQmuUSGa', 'user'),
(19, 'dfg', '$2b$06$X4GvT7weY7iE436ttv5uC.sXZQF8souvKzg1uhFdE5NwhUip6FV4K', 'user'),
(20, 'jjj', '$2b$06$tBL5uFDR.QybBpYfXsN2GuvRbxUgjCnudGuFK/IFs.d9BgHEjgDnC', 'user'),
(21, 'fff', '$2b$06$dPCef0p22gGdIsvR6Y0J/ugGDEjTvLhLn3OriJf8u2B/1BnApeuce', 'user'),
(22, 'user', '$2b$06$Xi0hbajTJsdRfq5W74JsKe96hlScnq70VRJEjeC1Tb58B.mG8Hvc.', 'user');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `operacje`
--

CREATE TABLE `operacje` (
  `id_operacji` int(11) NOT NULL,
  `id_wydzialu` int(11) NOT NULL,
  `kwota` decimal(11,2) NOT NULL,
  `opis` varchar(255) COLLATE utf8_polish_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Zrzut danych tabeli `operacje`
--

INSERT INTO `operacje` (`id_operacji`, `id_wydzialu`, `kwota`, `opis`) VALUES
(3, 2, '4500.00', 'Zakup podzespołów komputerowych'),
(17, 6, '9.99', 'czipsy dzalapino'),
(21, 5, '1200.00', 'qwe');

--
-- Wyzwalacze `operacje`
--
DELIMITER $$
CREATE TRIGGER `operacje_BEFORE_INSERT` BEFORE INSERT ON `operacje` FOR EACH ROW BEGIN

IF NEW.`id_wydzialu` IS NULL OR NEW.`id_wydzialu` = 0 OR NEW.`kwota` IS NULL OR TRIM(NEW.`kwota`) = "" OR NEW.`opis` IS NULL OR TRIM(NEW.`opis`) = ""
	THEN
		SIGNAL sqlstate '45000'
			SET MESSAGE_TEXT = "Missing data";
	END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `operacje_backup`
--

CREATE TABLE `operacje_backup` (
  `id_operacji` int(11) NOT NULL,
  `id_wydzialu` int(11) NOT NULL,
  `kwota` int(11) NOT NULL,
  `opis` varchar(255) COLLATE utf8_polish_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `pracownicy`
--

CREATE TABLE `pracownicy` (
  `id` int(11) NOT NULL,
  `imie` varchar(255) COLLATE utf8_polish_ci NOT NULL,
  `nazwisko` varchar(255) COLLATE utf8_polish_ci NOT NULL,
  `id_stanowiska` int(11) NOT NULL,
  `identyfikator_firmowy_pracownika` varchar(8) COLLATE utf8_polish_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Zrzut danych tabeli `pracownicy`
--

INSERT INTO `pracownicy` (`id`, `imie`, `nazwisko`, `id_stanowiska`, `identyfikator_firmowy_pracownika`) VALUES
(10, 'Michał', 'Nowak', 3, '32773891'),
(12, 'Anna', 'Kwiecień', 9, '43357029'),
(18, 'Jakub', 'Grabowski', 9, '87654321'),
(19, 'Jan', 'Kowalski', 11, '77456834'),
(20, 'Adam', 'Sadowski', 3, '12341234'),
(21, 'asd', 'asd', 3, '99999999');

--
-- Wyzwalacze `pracownicy`
--
DELIMITER $$
CREATE TRIGGER `pracownicy_BEFORE_INSERT` BEFORE INSERT ON `pracownicy` FOR EACH ROW BEGIN

IF NEW.`id` IS NULL OR NEW.`imie` IS NULL OR TRIM(NEW.`imie`) = "" OR NEW.`nazwisko` IS NULL OR TRIM(NEW.`nazwisko`) = "" OR NEW.`identyfikator_firmowy_pracownika` IS NULL OR TRIM(NEW.`identyfikator_firmowy_pracownika`) = "" OR NEW.`id_stanowiska` IS NULL
	THEN
		SIGNAL sqlstate '45000'
			SET MESSAGE_TEXT = "Missing data";
	END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `stanowiska`
--

CREATE TABLE `stanowiska` (
  `id` int(11) NOT NULL,
  `nazwa_stanowiska` varchar(255) COLLATE utf8_polish_ci NOT NULL,
  `pensja_bazowa` decimal(11,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Zrzut danych tabeli `stanowiska`
--

INSERT INTO `stanowiska` (`id`, `nazwa_stanowiska`, `pensja_bazowa`) VALUES
(3, 'Kierownik', '9200.00'),
(9, 'Informatyk', '6000.00'),
(11, 'Sprzedawca', '4500.00');

--
-- Wyzwalacze `stanowiska`
--
DELIMITER $$
CREATE TRIGGER `stanowiska_BEFORE_INSERT` BEFORE INSERT ON `stanowiska` FOR EACH ROW BEGIN

IF NEW.`id` IS NULL OR NEW.`nazwa_stanowiska` IS NULL OR TRIM(NEW.`nazwa_stanowiska`) = "" OR NEW.`pensja_bazowa` IS NULL 
	THEN
		SIGNAL sqlstate '45000'
			SET MESSAGE_TEXT = "Missing data";
	END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `wydzialy`
--

CREATE TABLE `wydzialy` (
  `id_wydzialu` int(11) NOT NULL,
  `nazwa_wydzialu` varchar(255) COLLATE utf8_polish_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Zrzut danych tabeli `wydzialy`
--

INSERT INTO `wydzialy` (`id_wydzialu`, `nazwa_wydzialu`) VALUES
(2, 'Dział IT'),
(3, 'Obsługa zagranicznych kontrahentów'),
(4, 'Dział inwestycji'),
(5, 'Logistyka'),
(6, 'Zamówienia firmowe');

--
-- Wyzwalacze `wydzialy`
--
DELIMITER $$
CREATE TRIGGER `wydzialy_BEFORE_INSERT` BEFORE INSERT ON `wydzialy` FOR EACH ROW BEGIN

IF NEW.`id_wydzialu` IS NULL OR NEW.`nazwa_wydzialu` IS NULL OR TRIM(NEW.`nazwa_wydzialu`) = "" 
	THEN
		SIGNAL sqlstate '45000'
			SET MESSAGE_TEXT = "Missing data";
	END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `wyplacone_pensje`
--

CREATE TABLE `wyplacone_pensje` (
  `id` int(11) NOT NULL,
  `id_pracownika` int(11) NOT NULL,
  `kwota` decimal(11,2) NOT NULL,
  `data` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Zrzut danych tabeli `wyplacone_pensje`
--

INSERT INTO `wyplacone_pensje` (`id`, `id_pracownika`, `kwota`, `data`) VALUES
(7, 18, '6203.00', '2022-12-30');

--
-- Wyzwalacze `wyplacone_pensje`
--
DELIMITER $$
CREATE TRIGGER `wyplacone_pensje_BEFORE_INSERT` BEFORE INSERT ON `wyplacone_pensje` FOR EACH ROW BEGIN

IF NEW.`id` IS NULL OR NEW.`id_pracownika` IS NULL OR NEW.`kwota` IS NULL OR NEW.`data` IS NULL
	THEN
		SIGNAL sqlstate '45000'
			SET MESSAGE_TEXT = "Missing data";
	END IF;
END
$$
DELIMITER ;

--
-- Indeksy dla zrzutów tabel
--

--
-- Indeksy dla tabeli `konta`
--
ALTER TABLE `konta`
  ADD PRIMARY KEY (`id_konta`);

--
-- Indeksy dla tabeli `konta_backup`
--
ALTER TABLE `konta_backup`
  ADD PRIMARY KEY (`id_konta`);

--
-- Indeksy dla tabeli `operacje`
--
ALTER TABLE `operacje`
  ADD PRIMARY KEY (`id_operacji`),
  ADD KEY `id_wydzialu` (`id_wydzialu`);

--
-- Indeksy dla tabeli `operacje_backup`
--
ALTER TABLE `operacje_backup`
  ADD PRIMARY KEY (`id_operacji`);

--
-- Indeksy dla tabeli `pracownicy`
--
ALTER TABLE `pracownicy`
  ADD PRIMARY KEY (`id`),
  ADD KEY `id_stanowiska` (`id_stanowiska`);

--
-- Indeksy dla tabeli `stanowiska`
--
ALTER TABLE `stanowiska`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `wydzialy`
--
ALTER TABLE `wydzialy`
  ADD PRIMARY KEY (`id_wydzialu`);

--
-- Indeksy dla tabeli `wyplacone_pensje`
--
ALTER TABLE `wyplacone_pensje`
  ADD PRIMARY KEY (`id`),
  ADD KEY `id_pracownika` (`id_pracownika`);

--
-- AUTO_INCREMENT dla zrzuconych tabel
--

--
-- AUTO_INCREMENT dla tabeli `konta`
--
ALTER TABLE `konta`
  MODIFY `id_konta` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;

--
-- AUTO_INCREMENT dla tabeli `operacje`
--
ALTER TABLE `operacje`
  MODIFY `id_operacji` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=22;

--
-- AUTO_INCREMENT dla tabeli `operacje_backup`
--
ALTER TABLE `operacje_backup`
  MODIFY `id_operacji` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT dla tabeli `pracownicy`
--
ALTER TABLE `pracownicy`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=22;

--
-- AUTO_INCREMENT dla tabeli `stanowiska`
--
ALTER TABLE `stanowiska`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;

--
-- AUTO_INCREMENT dla tabeli `wydzialy`
--
ALTER TABLE `wydzialy`
  MODIFY `id_wydzialu` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT dla tabeli `wyplacone_pensje`
--
ALTER TABLE `wyplacone_pensje`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- Ograniczenia dla zrzutów tabel
--

--
-- Ograniczenia dla tabeli `operacje`
--
ALTER TABLE `operacje`
  ADD CONSTRAINT `operacje_ibfk_1` FOREIGN KEY (`id_wydzialu`) REFERENCES `wydzialy` (`id_wydzialu`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Ograniczenia dla tabeli `pracownicy`
--
ALTER TABLE `pracownicy`
  ADD CONSTRAINT `pracownicy_ibfk_1` FOREIGN KEY (`id_stanowiska`) REFERENCES `stanowiska` (`id`);

--
-- Ograniczenia dla tabeli `wyplacone_pensje`
--
ALTER TABLE `wyplacone_pensje`
  ADD CONSTRAINT `wyplacone_pensje_ibfk_1` FOREIGN KEY (`id_pracownika`) REFERENCES `pracownicy` (`id`);

DELIMITER $$
--
-- Zdarzenia
--
CREATE DEFINER=`root`@`localhost` EVENT `wyczyszczenie tabeli operacje_backup` ON SCHEDULE EVERY 1 DAY STARTS '2022-06-14 01:00:00' ON COMPLETION NOT PRESERVE ENABLE DO TRUNCATE TABLE operacje_backup$$

CREATE DEFINER=`root`@`localhost` EVENT `kopia tabeli operacje` ON SCHEDULE EVERY 1 DAY STARTS '2022-06-14 01:00:40' ON COMPLETION NOT PRESERVE ENABLE DO INSERT INTO operacje_backup SELECT * FROM operacje$$

DELIMITER ;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
