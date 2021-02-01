-- MySQL dump 10.13  Distrib 5.7.15, for Linux (x86_64)
--
-- Host: localhost    Database: kasasrce
-- ------------------------------------------------------
-- Server version	5.7.15-0ubuntu0.16.04.1-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `art`
--

DROP TABLE IF EXISTS `art`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `art` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `artsif` varchar(30) NOT NULL,
  `barcode` varchar(30) DEFAULT NULL,
  `jmjid` int(10) unsigned DEFAULT NULL,
  `alias` varchar(100) DEFAULT NULL,
  `naziv` varchar(200) NOT NULL,
  `normativ` enum('N','Y') NOT NULL DEFAULT 'N',
  `grupaid` int(10) unsigned DEFAULT NULL,
  `vidljiv` enum('1','0') DEFAULT '1',
  `deleted` enum('1','0') NOT NULL DEFAULT '0',
  `poredak` int(10) unsigned NOT NULL DEFAULT '1000',
  `statgrupaid` int(10) unsigned NOT NULL DEFAULT '0',
  `usluga` enum('N','Y') NOT NULL DEFAULT 'N',
  `povratnanaknada` double(14,4) NOT NULL DEFAULT '0.0000',
  `tip` int(10) unsigned NOT NULL DEFAULT '1' COMMENT 'u buducnosti link na tipove artikala a sada: 1-standardni artikl 2-za potrebe programa (PREDUJAM avansno placanje)',
  `slikaid` int(10) unsigned DEFAULT NULL,
  `povratnanaknadaulazna` double(14,4) unsigned NOT NULL DEFAULT '0.0000',
  `kolicinapokomadu` double(14,4) unsigned NOT NULL DEFAULT '1.0000',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `artsif_UNIQUE` (`artsif`),
  KEY `fk_artjedinicemjereID_idx` (`jmjid`),
  KEY `fk_artgrupaID_idx` (`grupaid`),
  KEY `idx_art_poredak` (`poredak`),
  KEY `idx_art_vidljiv` (`vidljiv`),
  KEY `idx_aid` (`id`),
  KEY `fk_artartslikeID_idx` (`slikaid`),
  CONSTRAINT `fk_artartslikeID` FOREIGN KEY (`slikaid`) REFERENCES `artslike` (`id`) ON UPDATE NO ACTION,
  CONSTRAINT `fk_artgrupaID` FOREIGN KEY (`grupaid`) REFERENCES `grupe` (`id`) ON UPDATE CASCADE,
  CONSTRAINT `fk_artjedinicemjereID` FOREIGN KEY (`jmjid`) REFERENCES `mat_jedinicemjere` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=367 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `art_normativ_table`
--

DROP TABLE IF EXISTS `art_normativ_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `art_normativ_table` (
  `aid` int(10) DEFAULT NULL,
  `nid` int(10) DEFAULT NULL,
  `kol` double(14,4) DEFAULT NULL,
  KEY `idx_aid` (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `artiklporez`
--

DROP TABLE IF EXISTS `artiklporez`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artiklporez` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `porezid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `aid_porezid_UNIQUE` (`aid`,`porezid`),
  KEY `fk_artiklporezporezID_idx` (`porezid`),
  KEY `fk_artiklporezartID_idx` (`aid`),
  CONSTRAINT `fk_artiklporezartID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_artiklporezporezID` FOREIGN KEY (`porezid`) REFERENCES `porez` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=593 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `artiklporezulaz`
--

DROP TABLE IF EXISTS `artiklporezulaz`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artiklporezulaz` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `porezid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `aid_porezid_UNIQUE` (`aid`,`porezid`),
  KEY `fk_artiklporezulazporezID_idx` (`porezid`),
  KEY `fk_artiklporezulazartID_idx` (`aid`),
  CONSTRAINT `fk_artiklporezulazartID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_artiklporezulazporezID` FOREIGN KEY (`porezid`) REFERENCES `porez` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='tablica ulaznih poreza za artikl';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `artslike`
--

DROP TABLE IF EXISTS `artslike`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artslike` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `naziv` varchar(200) NOT NULL,
  `opis` varchar(200) NOT NULL,
  `imageSize` int(10) unsigned NOT NULL,
  `image` blob NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='slike koje mogu imati artikli';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `brojracuna`
--

DROP TABLE IF EXISTS `brojracuna`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `brojracuna` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `br` int(6) NOT NULL COMMENT 'broj racuna',
  `godina` int(4) NOT NULL COMMENT 'godina izrade racuna',
  `vrsta` enum('R','O','P','RN','PR') DEFAULT NULL COMMENT 'racun, otpremnica, ponuda, radni nalog, primka',
  `kasaid` int(10) NOT NULL COMMENT 'ID naplatnog uredaja',
  `oznpp` varchar(20) NOT NULL COMMENT 'Oznaka poslovnog prostora',
  `oznsljed` enum('N','P') NOT NULL COMMENT 'N - brojevi na nivou naplatnog uredaja, P - brojevi na nivou poslovnog prostora',
  `datum` datetime NOT NULL,
  `vrstareal` enum('M','V','O','P','RN','A','PR') DEFAULT NULL,
  `iddoc` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_brojracuna_brvrsta` (`br`,`vrsta`),
  KEY `idx_brojracuna_godina` (`godina`),
  KEY `idx_brojracuna_kasaidoznpp` (`kasaid`,`oznpp`)
) ENGINE=InnoDB AUTO_INCREMENT=4676 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `brojracuna_start`
--

DROP TABLE IF EXISTS `brojracuna_start`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `brojracuna_start` (
  `vrsta` enum('R','O','P','RN','PR') NOT NULL,
  `vrstareal` enum('M','V','O','P','RN','A','PR') NOT NULL,
  `br` int(10) unsigned NOT NULL DEFAULT '0',
  `godina` int(4) NOT NULL COMMENT 'godina izrade racuna',
  `kasaid` int(10) NOT NULL COMMENT 'ID naplatnog uredaja',
  `oznpp` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cjenik`
--

DROP TABLE IF EXISTS `cjenik`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cjenik` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `sifra` char(10) NOT NULL,
  `naziv` varchar(50) NOT NULL,
  `opis` varchar(250) NOT NULL,
  `glavni` enum('N','Y') NOT NULL,
  `aktivan` enum('N','Y') NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `naziv_UNIQUE` (`naziv`),
  UNIQUE KEY `sifra_UNIQUE` (`sifra`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cjenikskladisteveza`
--

DROP TABLE IF EXISTS `cjenikskladisteveza`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cjenikskladisteveza` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `sid` int(10) unsigned NOT NULL,
  `cid` int(10) unsigned NOT NULL,
  `isdefault` enum('N','Y') NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE_idx` (`id`),
  UNIQUE KEY `sidcid_UNIQUE` (`sid`,`cid`),
  KEY `fk_cjenikID_idx` (`cid`),
  KEY `fk_cjenikskladisteskladisteID_idx` (`sid`),
  CONSTRAINT `fk_cjenikskladistecjenikID` FOREIGN KEY (`cid`) REFERENCES `cjenik` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cjenikskladisteskladisteID` FOREIGN KEY (`sid`) REFERENCES `matsklad` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cjenikstavke`
--

DROP TABLE IF EXISTS `cjenikstavke`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cjenikstavke` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `cid` int(10) unsigned NOT NULL,
  `punacijena` double(14,4) unsigned NOT NULL,
  `veleprodajnacijena` double(14,4) unsigned NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `aid_sid_UNIQUE` (`aid`,`cid`),
  KEY `fk_cjenikID_idx` (`cid`),
  KEY `fk_artID_idx` (`aid`),
  KEY `idx_aid` (`aid`),
  KEY `idx_cid` (`cid`),
  CONSTRAINT `fk_cjenikstavkeartID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_cjenikstavkecjenikID` FOREIGN KEY (`cid`) REFERENCES `cjenik` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=655 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cjeniktjednischedule`
--

DROP TABLE IF EXISTS `cjeniktjednischedule`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cjeniktjednischedule` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `cid` int(10) unsigned NOT NULL,
  `vrijediod` time NOT NULL,
  `vrijedido` time NOT NULL,
  `dayofweek` int(10) unsigned NOT NULL COMMENT 'dayofweek (1 = Sunday, 2 = Monday, ..., 7 = Saturday)',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `fk_idx` (`cid`),
  KEY `cts_vriod_idx` (`vrijediod`),
  KEY `cts_vrido_idx` (`vrijedido`),
  KEY `cts_dow_idx` (`dayofweek`),
  CONSTRAINT `fk_cjeniktjednischedule_cjenik_cid` FOREIGN KEY (`cid`) REFERENCES `cjenik` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `company`
--

DROP TABLE IF EXISTS `company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `company` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `naziv` varchar(250) NOT NULL,
  `adresa` varchar(250) NOT NULL,
  `dobavljac` enum('Y','N') NOT NULL DEFAULT 'N',
  `oib` char(20) NOT NULL,
  `MB` char(13) NOT NULL DEFAULT '""',
  `sifra` int(10) unsigned NOT NULL DEFAULT '0',
  `rabat` double(4,2) NOT NULL DEFAULT '0.00',
  `opis` longtext NOT NULL,
  `mjesto` varchar(100) NOT NULL,
  `countrycode` char(2) NOT NULL DEFAULT 'HR',
  `email` varchar(250) DEFAULT NULL,
  `telefon` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `naziv_UNIQUE` (`naziv`),
  UNIQUE KEY `oib_UNIQUE` (`oib`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crm`
--

DROP TABLE IF EXISTS `crm`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `crm` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `datum_unosa` datetime DEFAULT NULL,
  `kupacid` int(10) DEFAULT '0',
  `prodavacid` int(10) DEFAULT '0',
  `unioid` int(10) DEFAULT '0',
  `kupac_naziv` varchar(250) DEFAULT NULL,
  `prodavac_naziv` varchar(250) DEFAULT NULL,
  `unio_naziv` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crmopis`
--

DROP TABLE IF EXISTS `crmopis`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `crmopis` (
  `crmid` int(10) DEFAULT NULL,
  `opis` longtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crmrac`
--

DROP TABLE IF EXISTS `crmrac`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `crmrac` (
  `crmid` int(10) DEFAULT '0',
  `vrsta` varchar(2) DEFAULT '',
  `rid` int(10) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `euvatincountry`
--

DROP TABLE IF EXISTS `euvatincountry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `euvatincountry` (
  `code` char(2) NOT NULL,
  `country` varchar(50) NOT NULL,
  `countrylocal` varchar(50) NOT NULL,
  `opis` varchar(50) NOT NULL,
  `pdvoznaka` varchar(10) NOT NULL,
  `vatregex` varchar(100) NOT NULL,
  PRIMARY KEY (`code`),
  UNIQUE KEY `code_UNIQUE` (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='VAT identification number (VATIN) country data';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `firma`
--

DROP TABLE IF EXISTS `firma`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `firma` (
  `naziv` varchar(100) DEFAULT NULL,
  `adresa` varchar(100) DEFAULT NULL,
  `grad` varchar(100) DEFAULT NULL,
  `zip` char(5) DEFAULT NULL,
  `tel` varchar(100) DEFAULT NULL,
  `oib` char(11) NOT NULL DEFAULT '0',
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `iban` char(21) DEFAULT NULL,
  `fax` varchar(25) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  `web` varchar(80) DEFAULT NULL,
  `banka` varchar(80) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `grupe`
--

DROP TABLE IF EXISTS `grupe`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grupe` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `opis` varchar(250) NOT NULL,
  `glavna` tinyint(1) NOT NULL DEFAULT '0',
  `ispis_kuhinja` tinyint(1) NOT NULL DEFAULT '0',
  `statisticka` enum('N','Y') NOT NULL DEFAULT 'N',
  PRIMARY KEY (`id`),
  KEY `idx_grupa_ispiskuhinja` (`ispis_kuhinja`),
  KEY `idx_grupa_statisticka` (`statisticka`),
  KEY `idx_grupa_glavna` (`glavna`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `grupe_norm`
--

DROP TABLE IF EXISTS `grupe_norm`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grupe_norm` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `maticna_grid` int(10) unsigned NOT NULL,
  `parent_grid` int(10) unsigned NOT NULL,
  `grupa_id` int(10) unsigned NOT NULL,
  `rb` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `grupeporez`
--

DROP TABLE IF EXISTS `grupeporez`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grupeporez` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `gid` int(10) unsigned NOT NULL,
  `porezid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `gid_porezid_UNIQUE` (`gid`,`porezid`),
  KEY `fk_grupeporezporezID_idx` (`porezid`),
  KEY `fk_grupeporezartID_idx` (`gid`),
  CONSTRAINT `fk_grupeporezgID` FOREIGN KEY (`gid`) REFERENCES `grupe` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_grupeporezporezID` FOREIGN KEY (`porezid`) REFERENCES `porez` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inventura1`
--

DROP TABLE IF EXISTS `inventura1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inventura1` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `datum` date NOT NULL COMMENT 'datum primke',
  `skladID` int(10) unsigned NOT NULL COMMENT 'skladiste ID',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inventura2`
--

DROP TABLE IF EXISTS `inventura2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inventura2` (
  `aid` int(10) unsigned NOT NULL,
  `artsif` varchar(30) DEFAULT NULL,
  `naziv` varchar(200) DEFAULT NULL,
  `stanje_sklad` double(18,4) DEFAULT NULL,
  `stanje_stvarno` double(18,4) DEFAULT NULL,
  `razlika` double(18,4) DEFAULT NULL,
  `jmj` char(5) NOT NULL DEFAULT '',
  `opis` longtext,
  `sid` int(10) DEFAULT NULL,
  `sklad_naziv` varchar(200) DEFAULT NULL,
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `iid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inventuratmp`
--

DROP TABLE IF EXISTS `inventuratmp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inventuratmp` (
  `aid` int(10) unsigned NOT NULL,
  `artsif` varchar(30) DEFAULT NULL,
  `naziv` varchar(200) DEFAULT NULL,
  `stanje_sklad` double(18,4) DEFAULT NULL,
  `stanje_stvarno` double(18,4) DEFAULT NULL,
  `razlika` double(18,4) DEFAULT NULL,
  `jmj` char(5) NOT NULL DEFAULT '',
  `opis` longtext,
  `sid` int(10) DEFAULT NULL,
  `sklad_naziv` varchar(200) DEFAULT NULL,
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=68 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `izvinv`
--

DROP TABLE IF EXISTS `izvinv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `izvinv` (
  `aid` int(10) unsigned DEFAULT NULL,
  `stanje` double(14,4) DEFAULT '0.0000',
  `ulaz` double(14,4) DEFAULT '0.0000',
  `izlaz` double(14,4) DEFAULT '0.0000',
  `ulaza_od` double(14,4) DEFAULT '0.0000',
  `izlaza_od` double(14,4) DEFAULT '0.0000',
  `stanjeOd` double(14,4) DEFAULT '0.0000',
  `stanje_datod` double(14,4) DEFAULT '0.0000'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `izvod`
--

DROP TABLE IF EXISTS `izvod`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `izvod` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `vrid` int(10) unsigned DEFAULT NULL COMMENT 'vrac1 ID',
  `datum_placanja` date DEFAULT NULL,
  `iznos` double(14,2) DEFAULT '0.00',
  `opis_placanja` varchar(200) DEFAULT NULL,
  `poziv` varchar(200) DEFAULT NULL COMMENT 'Poziv na broj',
  `izvodbr` int(10) unsigned DEFAULT '0' COMMENT 'broj izvoda',
  `izvodgod` int(10) unsigned DEFAULT NULL COMMENT 'godina izvoda',
  PRIMARY KEY (`id`),
  KEY `fk_izvod_vrac1_idx` (`vrid`),
  CONSTRAINT `fk_izvod_vrac1_vrid_id` FOREIGN KEY (`vrid`) REFERENCES `vrac1` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kartice`
--

DROP TABLE IF EXISTS `kartice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kartice` (
  `id` int(3) NOT NULL AUTO_INCREMENT,
  `naziv` varchar(200) NOT NULL,
  `alias` varchar(100) NOT NULL,
  `dozvola` enum('N','Y') NOT NULL,
  `fiskal_znak` char(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kasa_stanje`
--

DROP TABLE IF EXISTS `kasa_stanje`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kasa_stanje` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `datum` datetime DEFAULT NULL,
  `kasa_id` int(10) DEFAULT NULL,
  `korisnik_id` int(10) DEFAULT NULL,
  `iznos` double(14,4) DEFAULT NULL,
  `razlog` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `korisnik`
--

DROP TABLE IF EXISTS `korisnik`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `korisnik` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `naziv` varchar(100) NOT NULL,
  `PuniNaziv` varchar(250) NOT NULL,
  `pregled` enum('N','Y') NOT NULL,
  `sifra` varchar(100) NOT NULL,
  `dozvole` int(3) NOT NULL,
  `oib` char(11) NOT NULL,
  `active` enum('A','N','O') NOT NULL DEFAULT 'A' COMMENT 'A - aktivan, N - nekativan, O - obrisan',
  `rfID` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `naziv_UNIQUE` (`naziv`),
  UNIQUE KEY `PuniNaziv_UNIQUE` (`PuniNaziv`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `korisnik_boje`
--

DROP TABLE IF EXISTS `korisnik_boje`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `korisnik_boje` (
  `uid` int(10) DEFAULT NULL,
  `boja` varchar(250) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `korisnik_dodatno`
--

DROP TABLE IF EXISTS `korisnik_dodatno`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `korisnik_dodatno` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `uid` int(10) DEFAULT NULL,
  `sto` varchar(50) DEFAULT NULL,
  `value` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `korisnik_dozvole`
--

DROP TABLE IF EXISTS `korisnik_dozvole`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `korisnik_dozvole` (
  `dozvola` int(10) DEFAULT NULL,
  `opis` longtext,
  UNIQUE KEY `dozvola` (`dozvola`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locprimk1`
--

DROP TABLE IF EXISTS `locprimk1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locprimk1` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `dokbr` int(10) unsigned NOT NULL COMMENT 'broj dokumenta',
  `dobavljacID` int(10) unsigned NOT NULL COMMENT 'id dobavljaca',
  `datumdok` date NOT NULL COMMENT 'datum primke',
  `datumv` date DEFAULT NULL COMMENT 'datum placanja',
  `skladID` int(10) unsigned NOT NULL COMMENT 'skladiste ID',
  `tipfID` int(10) unsigned DEFAULT NULL COMMENT 'tuzemna ili inozemna',
  `vezadokument` varchar(100) DEFAULT NULL COMMENT 'opis dokumenta (racuna) na kojeg se primka odnosi',
  `fiznosstart` double(14,4) NOT NULL COMMENT 'puni iznos fakture (pocetni bez PDV)',
  `fiznosrabatkn` double(14,4) DEFAULT NULL COMMENT 'rabat u kn (iznos bez PDV)',
  `fiznosukupno` double(14,4) DEFAULT NULL COMMENT 'Fakturni iznos bez PDV - rabat',
  `fiznospdv` double(14,4) DEFAULT NULL COMMENT 'pdv ukupno',
  `ztiznososn` double(14,4) DEFAULT NULL COMMENT 'zavisni troskovi ',
  `ztiznospdv` double(14,4) DEFAULT NULL,
  `ztkoefi` double(14,4) DEFAULT NULL COMMENT 'koeficijent zavisnih troskova (zt/fiznosukupno)',
  `iznosukupno` double(14,4) DEFAULT NULL COMMENT 'fiznosukupno + svi troskovi (zt)',
  `valsifra` char(3) DEFAULT NULL COMMENT 'sifra valute',
  `valdatum` date DEFAULT NULL COMMENT 'datum tecajne liste',
  `valtecaj` double(14,4) DEFAULT NULL COMMENT 'tecaj jedinicni',
  `valiznos` double(14,4) DEFAULT NULL COMMENT 'iznos u valuti',
  `storno` enum('0','1') DEFAULT '0' COMMENT 'da li je dokument storno',
  `storno_datum` date DEFAULT NULL COMMENT 'datum storniranja',
  `kasaid` int(10) DEFAULT '0' COMMENT 'id kase (na kojoj je unesena primka) - za prikaz lokalnih dokumenata',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locprimk2`
--

DROP TABLE IF EXISTS `locprimk2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locprimk2` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dokid` int(10) unsigned NOT NULL,
  `aid` int(10) unsigned NOT NULL,
  `naziv` varchar(250) NOT NULL,
  `kolicina` double(14,4) NOT NULL,
  `jmj` char(5) NOT NULL DEFAULT '',
  `fcijenajedinicna` double(14,4) NOT NULL DEFAULT '0.0000',
  `fcijenaukupno` double(14,4) NOT NULL DEFAULT '0.0000',
  `frabatp` double(14,4) NOT NULL DEFAULT '0.0000',
  `frabatknjed` double(14,4) NOT NULL DEFAULT '0.0000',
  `frabatknukp` double(14,4) NOT NULL DEFAULT '0.0000',
  `fcijenaukupnoumanjeno` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncukupnoitroskovi` double(14,4) NOT NULL DEFAULT '0.0000',
  `marzap` double(14,4) NOT NULL DEFAULT '0.0000',
  `marzakn` double(14,4) NOT NULL DEFAULT '0.0000',
  `ztdokiznososn` double(14,4) NOT NULL DEFAULT '0.0000',
  `ztdokiznospdv` double(14,4) NOT NULL DEFAULT '0.0000',
  `ztdokkoefi` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncpdvukupno` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncpdvodbitakp` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncpdvodbitakkn` double(14,4) NOT NULL DEFAULT '0.0000',
  `pcjedinicna` double(14,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `fk_locprimk2locprimk1id_idx` (`dokid`),
  KEY `fk_locprimk2artid_idx` (`aid`),
  CONSTRAINT `fk_locprimk2artid` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_locprimk2primk1id` FOREIGN KEY (`dokid`) REFERENCES `locprimk1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locprimk3`
--

DROP TABLE IF EXISTS `locprimk3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locprimk3` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dokid` int(10) unsigned NOT NULL,
  `opis` varchar(250) NOT NULL,
  `iznososn` double(14,4) NOT NULL,
  `iznospdv` double(14,4) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `fk_locprimk3locprimk1id_idx` (`dokid`),
  CONSTRAINT `fk_locprimk3locprimk1id` FOREIGN KEY (`dokid`) REFERENCES `locprimk1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locrac1`
--

DROP TABLE IF EXISTS `locrac1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locrac1` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `br` int(6) NOT NULL,
  `vrsta` enum('M','V','O','P','A') NOT NULL COMMENT 'M-racun maloprodaja V-racun veleprodaja O-otpremnica P-ponuda A-racun za predujam',
  `idk` int(10) unsigned DEFAULT NULL,
  `sum` double(14,4) DEFAULT NULL,
  `bpdv` double(14,4) DEFAULT NULL,
  `datumr` datetime DEFAULT NULL,
  `uid` int(10) unsigned DEFAULT NULL,
  `obracun` enum('N','Y') DEFAULT NULL,
  `stol` varchar(10) DEFAULT NULL,
  `np` int(3) DEFAULT NULL,
  `storno` enum('0','1') DEFAULT '0',
  `rabatp` double(14,4) DEFAULT NULL,
  `rabatk` double(14,4) DEFAULT NULL,
  `rsum` double(14,4) DEFAULT NULL,
  `rbpdv` double(14,4) DEFAULT NULL,
  `storno_datum` datetime DEFAULT NULL,
  `oib` char(11) NOT NULL,
  `zki` char(32) NOT NULL,
  `sid` int(10) unsigned NOT NULL,
  `cid` int(10) unsigned NOT NULL,
  `kasaid` int(10) DEFAULT '0',
  `oznpp` varchar(20) DEFAULT '0',
  `datumv` date NOT NULL DEFAULT '0000-00-00',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locrac1opis`
--

DROP TABLE IF EXISTS `locrac1opis`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locrac1opis` (
  `rid` int(10) unsigned NOT NULL,
  `opis` varchar(1024) NOT NULL,
  PRIMARY KEY (`rid`),
  UNIQUE KEY `rid_UNIQUE` (`rid`),
  KEY `fk_locrac1opis_locrac1_idx_idx` (`rid`),
  CONSTRAINT `fk_locrac1opis_locrac1_idx` FOREIGN KEY (`rid`) REFERENCES `locrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locrac2`
--

DROP TABLE IF EXISTS `locrac2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locrac2` (
  `rid` int(10) unsigned NOT NULL,
  `aid` int(10) unsigned NOT NULL,
  `artsif` varchar(30) DEFAULT '',
  `jmjsifra` char(5) NOT NULL,
  `kol` double(14,4) NOT NULL,
  `nc` double(14,4) DEFAULT NULL,
  `pcbpdv` double(14,4) DEFAULT NULL,
  `pc` double(14,4) DEFAULT NULL,
  `ukupno` double(14,4) DEFAULT NULL,
  `bpdv` double(14,4) DEFAULT NULL,
  `rabatP` double(14,4) DEFAULT NULL,
  `rabatK` double(14,4) DEFAULT NULL,
  `ukpcisti` double(14,4) DEFAULT NULL,
  `naziv` varchar(200) DEFAULT NULL,
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `fk_rac2_rac1_idx` (`rid`),
  KEY `fk_rac2artID_idx` (`aid`),
  CONSTRAINT `fk_locrac2artID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_locrac2locrac1ID` FOREIGN KEY (`rid`) REFERENCES `locrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locrac2opis`
--

DROP TABLE IF EXISTS `locrac2opis`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locrac2opis` (
  `r2id` int(10) unsigned NOT NULL,
  `opis` text NOT NULL,
  PRIMARY KEY (`r2id`),
  UNIQUE KEY `r2id_UNIQUE` (`r2id`),
  KEY `fk_locrac2opis_locrac2_idx_idx` (`r2id`),
  CONSTRAINT `fk_locrac2opis_locrac2_idx` FOREIGN KEY (`r2id`) REFERENCES `locrac2` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `logovi`
--

DROP TABLE IF EXISTS `logovi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `logovi` (
  `id` int(6) NOT NULL AUTO_INCREMENT,
  `datum` datetime DEFAULT NULL,
  `sto` varchar(255) DEFAULT NULL,
  `tko` int(3) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lookup`
--

DROP TABLE IF EXISTS `lookup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lookup` (
  `pojam` char(2) NOT NULL,
  `sifra` char(3) NOT NULL,
  `opis` varchar(20) NOT NULL,
  `value1` double(14,7) DEFAULT NULL,
  `value2` double(14,7) DEFAULT NULL,
  `value3` double(14,7) DEFAULT '0.0000000',
  `value4` double(14,7) DEFAULT '0.0000000',
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mat_jedinicemjere`
--

DROP TABLE IF EXISTS `mat_jedinicemjere`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mat_jedinicemjere` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `sifra` char(5) DEFAULT NULL,
  `opis` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `sifra_UNIQUE` (`sifra`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `matsklad`
--

DROP TABLE IF EXISTS `matsklad`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `matsklad` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `sifra` char(10) NOT NULL,
  `naziv` char(50) NOT NULL,
  `opis` char(50) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `sifra_UNIQUE` (`sifra`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `msk1`
--

DROP TABLE IF EXISTS `msk1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `msk1` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `Opis` varchar(250) DEFAULT NULL,
  `SIDsa` int(10) NOT NULL,
  `SIDna` int(10) NOT NULL,
  `datum` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=37 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `msk2`
--

DROP TABLE IF EXISTS `msk2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `msk2` (
  `mskid` int(10) NOT NULL,
  `aid` int(10) DEFAULT NULL,
  `artsif` varchar(30) DEFAULT NULL,
  `naziv` varchar(100) DEFAULT NULL,
  `kolicina` double(14,4) DEFAULT NULL,
  `id` int(10) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=268 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `msk2_sklad`
AFTER INSERT ON `msk2`
FOR EACH ROW
BEGIN
	DECLARE _SkladSa int;
	DECLARE _SkladNa int;
	set _SkladSa=(select SIDsa from msk1 where id=NEW.mskid);
	set _SkladNa=(select SIDna from msk1 where id=NEW.mskid);
	call proc_Update_Skladista(NEW.aid,_SkladSa,(NEW.kolicina*-1),NEW.artsif);
	call proc_Update_Skladista(NEW.aid,_SkladNa,NEW.kolicina,NEW.artsif);
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `narudzba1`
--

DROP TABLE IF EXISTS `narudzba1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `narudzba1` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `br` int(10) DEFAULT NULL,
  `datumn` datetime DEFAULT NULL,
  `narucio_uid` int(10) DEFAULT NULL,
  `stol` int(10) DEFAULT NULL,
  `status` enum('otvorena','zatvorena','brisana') DEFAULT NULL,
  `racid` int(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `narudzba2`
--

DROP TABLE IF EXISTS `narudzba2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `narudzba2` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `narid` int(10) DEFAULT NULL,
  `aid` int(10) DEFAULT NULL,
  `artsif` varchar(30) DEFAULT NULL,
  `naziv` longtext,
  `kol` double(14,4) DEFAULT NULL,
  `nc` double(14,4) DEFAULT NULL,
  `pc` double(18,6) DEFAULT NULL,
  `ukupno` double(18,6) DEFAULT NULL,
  `rabatP` double(14,4) DEFAULT NULL,
  `rabatk` double(18,6) DEFAULT NULL,
  `ukpcisti` double(18,6) DEFAULT NULL,
  `bpdvcisti` double(18,6) DEFAULT NULL,
  `bpdv` double(18,6) DEFAULT NULL,
  `grupaid` int(10) DEFAULT NULL,
  `ispis_kuhinja` int(10) DEFAULT NULL,
  `uid` int(6) DEFAULT NULL,
  `povratnanaknada` double(14,4) DEFAULT '0.0000',
  `crmid` int(10) DEFAULT '0',
  `datedodano` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `nnn`
--

DROP TABLE IF EXISTS `nnn`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nnn` (
  `naz` varchar(200) DEFAULT NULL,
  `cijena` double(14,4) DEFAULT NULL,
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `aid` int(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=82 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `normativ`
--

DROP TABLE IF EXISTS `normativ`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `normativ` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `nid` int(10) unsigned NOT NULL,
  `kolicina` double(14,4) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `aid_nid_UNIQUE` (`aid`,`nid`),
  KEY `fk_normativchildartiklID_idx` (`nid`),
  KEY `fk_normativparentartiklID_idx` (`aid`),
  CONSTRAINT `fk_normativchildartiklID` FOREIGN KEY (`nid`) REFERENCES `art` (`id`) ON UPDATE CASCADE,
  CONSTRAINT `fk_normativparentartiklID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=265 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `obracun`
--

DROP TABLE IF EXISTS `obracun`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `obracun` (
  `aid` int(3) DEFAULT NULL,
  `artsif` varchar(30) DEFAULT NULL,
  `jmj` int(3) DEFAULT NULL,
  `nc` double(14,4) DEFAULT NULL,
  `pc` double(14,4) DEFAULT NULL,
  `pst` double(14,4) DEFAULT NULL,
  `ulaz` double(14,4) DEFAULT NULL,
  `izlaz` double(14,4) DEFAULT NULL,
  `zavrsno` double(14,4) DEFAULT NULL,
  `nvrr` double(14,4) DEFAULT NULL,
  `ukpvrr` double(14,4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `opcije`
--

DROP TABLE IF EXISTS `opcije`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `opcije` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Sto` varchar(250) DEFAULT NULL,
  `Sifra` varchar(250) DEFAULT NULL,
  `Opis` varchar(250) DEFAULT NULL,
  `Value1` varchar(250) DEFAULT NULL,
  `kasaID` int(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `porez`
--

DROP TABLE IF EXISTS `porez`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `porez` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `sifra` char(5) NOT NULL,
  `opis` varchar(50) NOT NULL,
  `postotak` double(14,7) NOT NULL,
  `faktorPunaCijenaPorez` double(14,7) NOT NULL,
  `faktorPunaCijenaOsnovica` double(14,7) NOT NULL,
  `poreztip` char(5) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `sifra_UNIQUE` (`sifra`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='Tablica gdje se nalaze podaci o svim trenutno definiranim porezima';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `poreztip`
--

DROP TABLE IF EXISTS `poreztip`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `poreztip` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `tip` char(3) NOT NULL,
  `opis` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `tip_UNIQUE` (`tip`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `poruke`
--

DROP TABLE IF EXISTS `poruke`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `poruke` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `poruka` longtext,
  `dozvoljen_rad` enum('0','1') DEFAULT NULL,
  `aktivna` enum('0','1') DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `primk1`
--

DROP TABLE IF EXISTS `primk1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `primk1` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `dokbr` int(10) unsigned NOT NULL COMMENT 'broj dokumenta',
  `dobavljacID` int(10) unsigned NOT NULL COMMENT 'id dobavljaca',
  `datumdok` date NOT NULL COMMENT 'datum primke',
  `datumv` date DEFAULT NULL COMMENT 'datum placanja',
  `skladID` int(10) unsigned NOT NULL COMMENT 'skladiste ID',
  `tipfID` int(10) unsigned DEFAULT NULL COMMENT 'tuzemna ili inozemna',
  `vezadokument` varchar(100) DEFAULT NULL COMMENT 'opis dokumenta (racuna) na kojeg se primka odnosi',
  `fiznosstart` double(14,4) NOT NULL COMMENT 'puni iznos fakture (pocetni bez PDV)',
  `fiznosrabatkn` double(14,4) DEFAULT NULL COMMENT 'rabat u kn (iznos bez PDV)',
  `fiznosukupno` double(14,4) DEFAULT NULL COMMENT 'Fakturni iznos bez PDV - rabat',
  `fiznospdv` double(14,4) DEFAULT NULL COMMENT 'pdv ukupno',
  `ztiznososn` double(14,4) DEFAULT NULL COMMENT 'zavisni troskovi ',
  `ztiznospdv` double(14,4) DEFAULT NULL,
  `ztkoefi` double(14,4) DEFAULT NULL COMMENT 'koeficijent zavisnih troskova (zt/fiznosukupno)',
  `iznosukupno` double(14,4) DEFAULT NULL COMMENT 'fiznosukupno + svi troskovi (zt)',
  `valsifra` char(3) DEFAULT NULL COMMENT 'sifra valute',
  `valdatum` date DEFAULT NULL COMMENT 'datum tecajne liste',
  `valtecaj` double(14,4) DEFAULT NULL COMMENT 'tecaj jedinicni',
  `valiznos` double(14,4) DEFAULT NULL COMMENT 'iznos u valuti',
  `storno` enum('0','1') DEFAULT '0' COMMENT 'da li je dokument storno',
  `storno_datum` date DEFAULT NULL COMMENT 'datum storniranja',
  `kasaid` int(10) DEFAULT '0' COMMENT 'id kase (na kojoj je unesena primka) - za prikaz lokalnih dokumenata',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=38 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `primk2`
--

DROP TABLE IF EXISTS `primk2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `primk2` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dokid` int(10) unsigned NOT NULL,
  `aid` int(10) unsigned NOT NULL,
  `naziv` varchar(250) NOT NULL,
  `kolicina` double(14,4) NOT NULL,
  `jmj` char(5) NOT NULL DEFAULT '',
  `fcijenajedinicna` double(14,4) NOT NULL DEFAULT '0.0000',
  `fcijenaukupno` double(14,4) NOT NULL DEFAULT '0.0000',
  `frabatp` double(14,4) NOT NULL DEFAULT '0.0000',
  `frabatknjed` double(14,4) NOT NULL DEFAULT '0.0000',
  `frabatknukp` double(14,4) NOT NULL DEFAULT '0.0000',
  `fcijenaukupnoumanjeno` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncukupnoitroskovi` double(14,4) NOT NULL DEFAULT '0.0000',
  `marzap` double(14,4) NOT NULL DEFAULT '0.0000',
  `marzakn` double(14,4) NOT NULL DEFAULT '0.0000',
  `ztdokiznososn` double(14,4) NOT NULL DEFAULT '0.0000',
  `ztdokiznospdv` double(14,4) NOT NULL DEFAULT '0.0000',
  `ztdokkoefi` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncpdvukupno` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncpdvodbitakp` double(14,4) NOT NULL DEFAULT '0.0000',
  `ncpdvodbitakkn` double(14,4) NOT NULL DEFAULT '0.0000',
  `pcjedinicna` double(14,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `fk_primk2primk1id_idx` (`dokid`),
  KEY `fk_primk2artid_idx` (`aid`),
  CONSTRAINT `fk_primk2artid` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_primk2primk1id` FOREIGN KEY (`dokid`) REFERENCES `primk1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=224 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `pr2`
AFTER INSERT ON `primk2`
FOR EACH ROW
BEGIN
	DECLARE _Sklad int;
	DECLARE _Artsif varchar(30);
	DECLARE _ArtUsluga char;
	SET _ArtUsluga=(SELECT usluga FROM art WHERE id=NEW.aid);
	IF (_ArtUsluga='N') THEN
		set _Artsif=(select artsif from art where id=NEW.aid);
		set _Sklad=(select skladID from primk1 where id=NEW.dokid);
		call proc_Update_Skladista(NEW.aid,_Sklad,NEW.kolicina,_Artsif);
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `primk3`
--

DROP TABLE IF EXISTS `primk3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `primk3` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dokid` int(10) unsigned NOT NULL,
  `opis` varchar(250) NOT NULL,
  `iznososn` double(14,4) NOT NULL,
  `iznospdv` double(14,4) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `fk_primk3primk1id_idx` (`dokid`),
  CONSTRAINT `fk_primk3primk1id` FOREIGN KEY (`dokid`) REFERENCES `primk1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `printer_postavke`
--

DROP TABLE IF EXISTS `printer_postavke`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `printer_postavke` (
  `prnid` int(10) NOT NULL,
  `funkcija` varchar(45) NOT NULL,
  `prefix` varchar(100) DEFAULT NULL,
  `prefix_nastavak` varchar(45) DEFAULT NULL,
  `suffix` varchar(100) DEFAULT NULL,
  `suffix_nastavak` varchar(45) DEFAULT NULL,
  `id` int(10) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `printeri`
--

DROP TABLE IF EXISTS `printeri`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `printeri` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `naziv` varchar(45) DEFAULT NULL,
  `opisPrintera` longtext,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac1`
--

DROP TABLE IF EXISTS `rac1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac1` (
  `id` int(6) NOT NULL AUTO_INCREMENT,
  `br` int(6) DEFAULT NULL,
  `vrsta` enum('M','V') DEFAULT NULL,
  `idk` int(6) DEFAULT NULL,
  `sum` double(14,4) DEFAULT NULL,
  `bpdv` double(14,4) DEFAULT NULL,
  `datumr` datetime DEFAULT NULL,
  `uid` int(6) DEFAULT NULL,
  `obracun` enum('N','Y') DEFAULT NULL,
  `stol` varchar(10) DEFAULT NULL,
  `np` int(3) DEFAULT NULL,
  `storno` enum('0','1') DEFAULT '0',
  `rabatp` double(14,4) DEFAULT NULL,
  `rabatk` double(14,4) DEFAULT NULL,
  `rsum` double(14,4) DEFAULT NULL,
  `rbpdv` double(14,4) DEFAULT NULL,
  `storno_datum` datetime DEFAULT NULL,
  `oib` char(11) NOT NULL,
  `zki` char(32) NOT NULL,
  `sid` int(10) NOT NULL,
  `cid` int(10) NOT NULL,
  `kasaid` int(10) DEFAULT NULL,
  `oznPP` varchar(20) DEFAULT NULL,
  `narudzba` int(1) DEFAULT '0',
  `narucio_uid` int(10) DEFAULT NULL,
  `narid` int(10) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `idx_datumr` (`datumr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac1fiskal`
--

DROP TABLE IF EXISTS `rac1fiskal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac1fiskal` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `racid` int(10) DEFAULT NULL,
  `uuid` char(36) NOT NULL,
  `datum_slanja` datetime NOT NULL,
  `oib_obveznika` char(11) NOT NULL,
  `u_sustavu_pdv` tinyint(4) NOT NULL DEFAULT '1',
  `oznaka_sljednosti` char(1) NOT NULL DEFAULT 'P' COMMENT 'moze biti \\''P\\''-na nivou polslovnog prostora,\\''N\\''-na nivou naplatnog uredjaja\\n',
  `broj_racuna` text NOT NULL COMMENT 'propisani izgled broja racuna\\nbrojracuna/oznaka poslovnog prostora/oznaka naplatnog uredjaja\\n',
  `oznaka_naknadne_dostave` tinyint(4) NOT NULL DEFAULT '0',
  `oznaka_paragon_racuna` varchar(100) DEFAULT NULL,
  `specificna_namjena` varchar(1000) DEFAULT NULL,
  `odgovor` text COMMENT 'opis odogovra jir ili greske\\n',
  `potvrden` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `idx_racid` (`racid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac1opis`
--

DROP TABLE IF EXISTS `rac1opis`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac1opis` (
  `rid` int(10) DEFAULT NULL,
  `opis` longtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac1porezi`
--

DROP TABLE IF EXISTS `rac1porezi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac1porezi` (
  `rid` int(10) NOT NULL,
  `pid` int(10) unsigned DEFAULT NULL,
  `porez_sifra` char(5) DEFAULT NULL,
  `iznos_poreza` double(14,4) DEFAULT NULL,
  `porez_postotak` double(14,7) DEFAULT '0.0000000',
  `osnovica` double(18,6) DEFAULT '0.000000',
  KEY `idx_rid` (`rid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac2`
--

DROP TABLE IF EXISTS `rac2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac2` (
  `rid` int(6) DEFAULT NULL,
  `aid` int(9) DEFAULT NULL,
  `artsif` varchar(30) DEFAULT NULL,
  `kol` double(14,4) DEFAULT NULL,
  `nc` double(14,4) DEFAULT NULL,
  `pc` double(14,4) DEFAULT NULL,
  `ukupno` double(14,4) DEFAULT NULL,
  `bpdv` double(14,4) DEFAULT NULL,
  `rabatP` double(14,4) DEFAULT NULL,
  `rabatK` double(14,4) DEFAULT NULL,
  `ukpcisti` double(14,4) DEFAULT NULL,
  `bpdvcisti` double(18,6) DEFAULT NULL,
  `naziv` varchar(200) DEFAULT NULL,
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `r2localid` int(10) DEFAULT NULL,
  `povratna` double(14,4) DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  KEY `idx_rid` (`rid`),
  KEY `idx_aid` (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `trigg_rac2_normativ`
AFTER INSERT ON `rac2`
FOR EACH ROW
BEGIN
  CALL proc_rac3_insert_normativ(NEW.rid,NEW.aid,NEW.aid,NEW.kol);
  INSERT INTO rac2dodatno (rid,aid,kol,povratnanaknada) select NEW.rid,NEW.aid,NEW.kol,a.povratnanaknada from art a where a.id=NEW.aid;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `rac2dodatno`
--

DROP TABLE IF EXISTS `rac2dodatno`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac2dodatno` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `rid` int(10) DEFAULT NULL,
  `aid` int(10) DEFAULT NULL,
  `kol` double(14,4) DEFAULT '0.0000',
  `povratnanaknada` double(14,4) DEFAULT '0.0000',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac2porezi`
--

DROP TABLE IF EXISTS `rac2porezi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac2porezi` (
  `rid` int(10) NOT NULL,
  `aid` int(10) NOT NULL,
  `pid` int(10) NOT NULL,
  `koeficijent_poreza` double(14,6) NOT NULL DEFAULT '0.000000',
  `iznos_poreza` double(14,6) NOT NULL DEFAULT '0.000000',
  `porez_opis` char(5) DEFAULT NULL,
  `porez_postotak` double(14,7) NOT NULL DEFAULT '0.0000000',
  `r2id` int(10) DEFAULT NULL,
  `osnovica` double(14,6) NOT NULL DEFAULT '0.000000',
  `r2localid` int(10) DEFAULT NULL,
  KEY `idx_pid` (`pid`),
  KEY `idx_rid` (`rid`),
  KEY `idx_aid` (`aid`),
  KEY `idx_r2id` (`r2id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac3`
--

DROP TABLE IF EXISTS `rac3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac3` (
  `rid` int(6) DEFAULT NULL,
  `aid` int(9) DEFAULT NULL,
  `nid` int(9) DEFAULT NULL,
  `kol` double(14,4) DEFAULT NULL,
  KEY `idx_aid` (`aid`),
  KEY `idx_rid` (`rid`),
  KEY `idx_nid` (`nid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `rac3_sklad`
AFTER INSERT ON `rac3`
FOR EACH ROW
BEGIN
	DECLARE _Sklad int;
	DECLARE _Artsif varchar(30);
	DECLARE _ArtUsluga char;
	SET _ArtUsluga=(SELECT usluga FROM art WHERE id=NEW.nid);
	if (_ArtUsluga='N') THEN
		set _Artsif=(SELECT artsif FROM art WHERE id=NEW.nid);
		set _Sklad=(SELECT sid FROM rac1 WHERE id=NEW.rid);
		call proc_Update_Skladista(NEW.nid,_Sklad,(NEW.kol*-1),_Artsif);
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `rnalog1`
--

DROP TABLE IF EXISTS `rnalog1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rnalog1` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dok` int(10) unsigned DEFAULT NULL,
  `datumn` datetime DEFAULT NULL,
  `datumz` datetime DEFAULT NULL,
  `kid` int(10) unsigned DEFAULT NULL,
  `auto_id` int(10) DEFAULT NULL,
  `zatvoren` tinyint(1) DEFAULT NULL,
  `kupac_opis` varchar(250) DEFAULT NULL,
  `vozilo_opis` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rnalog2`
--

DROP TABLE IF EXISTS `rnalog2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rnalog2` (
  `rid` int(10) unsigned DEFAULT NULL,
  `opis_narucenih` longtext,
  `opis_napomena` longtext,
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rnalog3`
--

DROP TABLE IF EXISTS `rnalog3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rnalog3` (
  `rid` int(10) unsigned DEFAULT NULL,
  `aid` int(10) unsigned DEFAULT NULL,
  `artsif` varchar(30) DEFAULT NULL,
  `kol` double(14,4) DEFAULT NULL,
  `naziv` longtext,
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `sid` int(10) unsigned DEFAULT '0',
  `skinuto` tinyint(4) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `rnalog3_sklad`
AFTER UPDATE ON `rnalog3`
FOR EACH ROW
BEGIN
	DECLARE _Sklad int;
	DECLARE _Artsif varchar(30);
	DECLARE _ArtUsluga char;
	
	SET _ArtUsluga=(SELECT usluga FROM art WHERE id=NEW.aid);
	IF (NEW.skinuto=1) then
		IF (_ArtUsluga='N') THEN
			SET _Artsif=(SELECT artsif FROM art WHERE id=NEW.aid);
			CALL proc_Update_Skladista(NEW.aid,NEW.sid,(NEW.kol*-1),_Artsif);
		END IF;
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `serbr`
--

DROP TABLE IF EXISTS `serbr`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `serbr` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `vrsta` enum('primk','rac3','vrac3','msk3','rnalog3') DEFAULT NULL,
  `ulazizlaz` enum('U','I') DEFAULT NULL,
  `imanema` enum('Y','N') DEFAULT 'Y',
  `datum` datetime DEFAULT NULL,
  `dokid` int(10) unsigned DEFAULT NULL,
  `sid` int(10) unsigned DEFAULT NULL,
  `aid` int(10) unsigned DEFAULT NULL,
  `kol` double(14,4) NOT NULL DEFAULT '0.0000',
  `nc` double(14,4) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_datum` (`datum`),
  KEY `idx_dokid` (`dokid`),
  KEY `idx_aid` (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sklad`
--

DROP TABLE IF EXISTS `sklad`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sklad` (
  `id` int(6) NOT NULL AUTO_INCREMENT,
  `artsif` varchar(30) DEFAULT NULL,
  `aid` int(9) DEFAULT NULL,
  `sid` int(3) DEFAULT NULL,
  `kol` double(14,4) DEFAULT NULL,
  `pcijena` double(14,4) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_aid` (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `skladnadan`
--

DROP TABLE IF EXISTS `skladnadan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skladnadan` (
  `aid` int(10) DEFAULT NULL,
  `ulaz` double(14,4) DEFAULT '0.0000',
  `izlaz` double(14,4) DEFAULT '0.0000',
  `stanje` double(14,4) DEFAULT '0.0000'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `skladtmp`
--

DROP TABLE IF EXISTS `skladtmp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skladtmp` (
  `id` int(6) NOT NULL AUTO_INCREMENT,
  `artsif` varchar(30) DEFAULT NULL,
  `aid` int(9) DEFAULT NULL,
  `sid` int(3) DEFAULT NULL,
  `kol` double(14,4) DEFAULT NULL,
  `pcijena` double(14,4) DEFAULT NULL,
  `ulaz` double(14,4) DEFAULT NULL,
  `izlaz` double(14,4) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `t_viz_rac`
--

DROP TABLE IF EXISTS `t_viz_rac`;
/*!50001 DROP VIEW IF EXISTS `t_viz_rac`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `t_viz_rac` AS SELECT 
 1 AS `id`,
 1 AS `artsif`,
 1 AS `naziv`,
 1 AS `punacijena`,
 1 AS `stanje`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `grupa`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `test`
--

DROP TABLE IF EXISTS `test`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test` (
  `n` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tipf`
--

DROP TABLE IF EXISTS `tipf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tipf` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `tipf` int(10) unsigned NOT NULL,
  `opis` char(100) NOT NULL DEFAULT '',
  `opisskr` char(50) NOT NULL DEFAULT '',
  `PDVk` enum('Y','N') NOT NULL DEFAULT 'N',
  `Neopor` enum('Y','N') NOT NULL DEFAULT 'N',
  `OslobIzvoz` enum('Y','N') NOT NULL DEFAULT 'N',
  `OslobOst` enum('Y','N') NOT NULL DEFAULT 'N',
  `PoOsn` enum('Y','N') NOT NULL DEFAULT 'N',
  `poPDV` enum('Y','N') NOT NULL DEFAULT 'N',
  `kpOsn` enum('Y','N') NOT NULL DEFAULT 'N',
  `kpPDV` enum('Y','N') NOT NULL DEFAULT 'N',
  `pokazi` enum('Y','N') NOT NULL DEFAULT 'N',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `tipf_UNIQUE` (`tipf`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tpp`
--

DROP TABLE IF EXISTS `tpp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tpp` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `opis` varchar(250) NOT NULL,
  `porez` varchar(15) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ulaznecijene`
--

DROP TABLE IF EXISTS `ulaznecijene`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ulaznecijene` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `cijenaulaz` double(14,4) unsigned NOT NULL,
  `cijenaulazBPDV` double(14,4) unsigned NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `aid_sid_UNIQUE` (`aid`),
  KEY `fk_artID_idx` (`aid`),
  KEY `idx_aid` (`aid`),
  CONSTRAINT `fk_ulaznecijeneartID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Nabavne cijene artikala (pomocne cijene za unos primki)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `v_a_cjenik`
--

DROP TABLE IF EXISTS `v_a_cjenik`;
/*!50001 DROP VIEW IF EXISTS `v_a_cjenik`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_a_cjenik` AS SELECT 
 1 AS `aid`,
 1 AS `punacijena`,
 1 AS `cid`,
 1 AS `id`,
 1 AS `naziv`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_a_csv`
--

DROP TABLE IF EXISTS `v_a_csv`;
/*!50001 DROP VIEW IF EXISTS `v_a_csv`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_a_csv` AS SELECT 
 1 AS `aid`,
 1 AS `kol`,
 1 AS `sid`,
 1 AS `cid`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_a_pcj`
--

DROP TABLE IF EXISTS `v_a_pcj`;
/*!50001 DROP VIEW IF EXISTS `v_a_pcj`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_a_pcj` AS SELECT 
 1 AS `aid`,
 1 AS `punacijena`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `isdefault`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_a_prodano`
--

DROP TABLE IF EXISTS `v_a_prodano`;
/*!50001 DROP VIEW IF EXISTS `v_a_prodano`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_a_prodano` AS SELECT 
 1 AS `aid`,
 1 AS `prodano`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_d_primk1`
--

DROP TABLE IF EXISTS `v_d_primk1`;
/*!50001 DROP VIEW IF EXISTS `v_d_primk1`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_d_primk1` AS SELECT 
 1 AS `id`,
 1 AS `dokbr`,
 1 AS `skladID`,
 1 AS `dobavljac`,
 1 AS `naziv`,
 1 AS `vezadokument`,
 1 AS `datumdok`,
 1 AS `fiznosstart`,
 1 AS `fiznosrabatkn`,
 1 AS `fiznosukupno`,
 1 AS `fiznospdv`,
 1 AS `ztiznososn`,
 1 AS `iznosukupno`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_d_primk2`
--

DROP TABLE IF EXISTS `v_d_primk2`;
/*!50001 DROP VIEW IF EXISTS `v_d_primk2`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_d_primk2` AS SELECT 
 1 AS `dokid`,
 1 AS `aid`,
 1 AS `naziv`,
 1 AS `imj`,
 1 AS `kolicina`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_d_rac2`
--

DROP TABLE IF EXISTS `v_d_rac2`;
/*!50001 DROP VIEW IF EXISTS `v_d_rac2`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_d_rac2` AS SELECT 
 1 AS `rid`,
 1 AS `aid`,
 1 AS `naziv`,
 1 AS `kol`,
 1 AS `nc`,
 1 AS `pc`,
 1 AS `ukupno`,
 1 AS `bpdv`,
 1 AS `rabatP`,
 1 AS `rabatK`,
 1 AS `ukpcisti`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_ispis_primk1`
--

DROP TABLE IF EXISTS `v_ispis_primk1`;
/*!50001 DROP VIEW IF EXISTS `v_ispis_primk1`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_ispis_primk1` AS SELECT 
 1 AS `id`,
 1 AS `p1dokbr`,
 1 AS `p1datum`,
 1 AS `p1datumvaluta`,
 1 AS `p1veza`,
 1 AS `dob_naziv`,
 1 AS `dob_adresa`,
 1 AS `dob_mjesto`,
 1 AS `dob_oib`,
 1 AS `fiznosukupno`,
 1 AS `fiznospdv`,
 1 AS `ztiznososn`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_ispis_rac1_1`
--

DROP TABLE IF EXISTS `v_ispis_rac1_1`;
/*!50001 DROP VIEW IF EXISTS `v_ispis_rac1_1`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_ispis_rac1_1` AS SELECT 
 1 AS `r1id`,
 1 AS `sklad_naziv`,
 1 AS `cjenik_naziv`,
 1 AS `br`,
 1 AS `vrsta`,
 1 AS `idk`,
 1 AS `sum`,
 1 AS `bpdv`,
 1 AS `datumr`,
 1 AS `uid`,
 1 AS `obracun`,
 1 AS `stol`,
 1 AS `np`,
 1 AS `storno`,
 1 AS `rabatp`,
 1 AS `rabatk`,
 1 AS `rsum`,
 1 AS `rbpdv`,
 1 AS `storno_datum`,
 1 AS `oib`,
 1 AS `zki`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `kasaid`,
 1 AS `oznpp`,
 1 AS `kupac_naziv`,
 1 AS `kupac_adresa`,
 1 AS `kupac_mjesto`,
 1 AS `kupac_oib`,
 1 AS `jir`,
 1 AS `nacin_placanja`,
 1 AS `prodao`,
 1 AS `countrycode`,
 1 AS `country`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_ispis_rac1_v`
--

DROP TABLE IF EXISTS `v_ispis_rac1_v`;
/*!50001 DROP VIEW IF EXISTS `v_ispis_rac1_v`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_ispis_rac1_v` AS SELECT 
 1 AS `r1id`,
 1 AS `sklad_naziv`,
 1 AS `cjenik_naziv`,
 1 AS `br`,
 1 AS `vrsta`,
 1 AS `idk`,
 1 AS `sum`,
 1 AS `bpdv`,
 1 AS `datumr`,
 1 AS `uid`,
 1 AS `obracun`,
 1 AS `stol`,
 1 AS `np`,
 1 AS `storno`,
 1 AS `rabatp`,
 1 AS `rabatk`,
 1 AS `rsum`,
 1 AS `rbpdv`,
 1 AS `storno_datum`,
 1 AS `oib`,
 1 AS `zki`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `kasaid`,
 1 AS `oznpp`,
 1 AS `kupac_naziv`,
 1 AS `kupac_adresa`,
 1 AS `kupac_mjesto`,
 1 AS `kupac_oib`,
 1 AS `jir`,
 1 AS `nacin_placanja`,
 1 AS `prodao`,
 1 AS `datumv`,
 1 AS `countrycode`,
 1 AS `country`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_ispis_vrac1_porezi`
--

DROP TABLE IF EXISTS `v_ispis_vrac1_porezi`;
/*!50001 DROP VIEW IF EXISTS `v_ispis_vrac1_porezi`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_ispis_vrac1_porezi` AS SELECT 
 1 AS `rid`,
 1 AS `pid`,
 1 AS `porez_sifra`,
 1 AS `iznos_poreza`,
 1 AS `porez_postotak`,
 1 AS `postotak`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_izv_grupe_gn`
--

DROP TABLE IF EXISTS `v_izv_grupe_gn`;
/*!50001 DROP VIEW IF EXISTS `v_izv_grupe_gn`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_izv_grupe_gn` AS SELECT 
 1 AS `gid`,
 1 AS `opis`,
 1 AS `gru
pa_id`,
 1 AS `maticna`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_izv_po_grpid`
--

DROP TABLE IF EXISTS `v_izv_po_grpid`;
/*!50001 DROP VIEW IF EXISTS `v_izv_po_grpid`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_izv_po_grpid` AS SELECT 
 1 AS `id`,
 1 AS `datumr`,
 1 AS `np`,
 1 AS `vrsta`,
 1 AS `aid`,
 1 AS `grupaid`,
 1 AS `porez`,
 1 AS `osnovica`,
 1 AS `iznos`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_kasa_pregleddok`
--

DROP TABLE IF EXISTS `v_kasa_pregleddok`;
/*!50001 DROP VIEW IF EXISTS `v_kasa_pregleddok`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_kasa_pregleddok` AS SELECT 
 1 AS `id`,
 1 AS `br`,
 1 AS `broj_racuna`,
 1 AS `vrsta`,
 1 AS `datumr`,
 1 AS `sum`,
 1 AS `bpdv`,
 1 AS `rabatp`,
 1 AS `rabatk`,
 1 AS `alias`,
 1 AS `prodavac`,
 1 AS `kupac`,
 1 AS `stol`,
 1 AS `storno`,
 1 AS `rsum`,
 1 AS `rbpdv`,
 1 AS `zki`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `oznpp`,
 1 AS `kasaid`,
 1 AS `storno_datum`,
 1 AS `uid`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_xml_raczahtjev`
--

DROP TABLE IF EXISTS `v_xml_raczahtjev`;
/*!50001 DROP VIEW IF EXISTS `v_xml_raczahtjev`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_xml_raczahtjev` AS SELECT 
 1 AS `id`,
 1 AS `br`,
 1 AS `vrsta`,
 1 AS `idk`,
 1 AS `sum`,
 1 AS `bpdv`,
 1 AS `datumr`,
 1 AS `uid`,
 1 AS `obracun`,
 1 AS `stol`,
 1 AS `np`,
 1 AS `storno`,
 1 AS `rabatp`,
 1 AS `rabatk`,
 1 AS `rsum`,
 1 AS `rbpdv`,
 1 AS `storno_datum`,
 1 AS `oib`,
 1 AS `zki`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `porez_sifra`,
 1 AS `iz`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_xml_raczahtjev_porez`
--

DROP TABLE IF EXISTS `v_xml_raczahtjev_porez`;
/*!50001 DROP VIEW IF EXISTS `v_xml_raczahtjev_porez`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `v_xml_raczahtjev_porez` AS SELECT 
 1 AS `rid`,
 1 AS `aid`,
 1 AS `osnovica`,
 1 AS `iznos`,
 1 AS `pid`,
 1 AS `postotak`,
 1 AS `tip`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `view_stanje_skladista`
--

DROP TABLE IF EXISTS `view_stanje_skladista`;
/*!50001 DROP VIEW IF EXISTS `view_stanje_skladista`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `view_stanje_skladista` AS SELECT 
 1 AS `artsif`,
 1 AS `naziv`,
 1 AS `stanje`,
 1 AS `sid`,
 1 AS `skladiste`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viz_porezi`
--

DROP TABLE IF EXISTS `viz_porezi`;
/*!50001 DROP VIEW IF EXISTS `viz_porezi`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viz_porezi` AS SELECT 
 1 AS `aid`,
 1 AS `postotak`,
 1 AS `koef`,
 1 AS `id`,
 1 AS `sifra`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viz_rac`
--

DROP TABLE IF EXISTS `viz_rac`;
/*!50001 DROP VIEW IF EXISTS `viz_rac`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viz_rac` AS SELECT 
 1 AS `id`,
 1 AS `artsif`,
 1 AS `naziv`,
 1 AS `PC`,
 1 AS `stanje`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `grupaid`,
 1 AS `opis`,
 1 AS `ispis_kuhinja`,
 1 AS `povratnanaknada`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viz_rac_top20`
--

DROP TABLE IF EXISTS `viz_rac_top20`;
/*!50001 DROP VIEW IF EXISTS `viz_rac_top20`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viz_rac_top20` AS SELECT 
 1 AS `id`,
 1 AS `artsif`,
 1 AS `naziv`,
 1 AS `PC`,
 1 AS `prodano`,
 1 AS `sid`,
 1 AS `cid`,
 1 AS `grupaid`,
 1 AS `opis`,
 1 AS `ispis_kuhinja`,
 1 AS `povratnanaknada`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `vrac1`
--

DROP TABLE IF EXISTS `vrac1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac1` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `br` int(6) NOT NULL,
  `vrsta` enum('M','V','O','P','A') NOT NULL COMMENT 'M-racun maloprodaja V-racun veleprodaja O-otpremnica P-ponuda A-racun za predujam',
  `idk` int(10) unsigned DEFAULT NULL,
  `sum` double(14,4) DEFAULT NULL,
  `bpdv` double(14,4) DEFAULT NULL,
  `datumr` datetime DEFAULT NULL,
  `uid` int(10) unsigned DEFAULT NULL,
  `obracun` enum('N','Y') DEFAULT NULL,
  `stol` varchar(10) DEFAULT NULL,
  `np` int(3) DEFAULT NULL,
  `storno` enum('0','1') DEFAULT '0',
  `rabatp` double(14,4) DEFAULT NULL,
  `rabatk` double(14,4) DEFAULT NULL,
  `rsum` double(14,4) DEFAULT NULL,
  `rbpdv` double(14,4) DEFAULT NULL,
  `storno_datum` datetime DEFAULT NULL,
  `oib` char(11) NOT NULL,
  `zki` char(32) NOT NULL,
  `sid` int(10) unsigned NOT NULL,
  `cid` int(10) unsigned NOT NULL,
  `kasaid` int(10) DEFAULT '0',
  `oznpp` varchar(20) DEFAULT '0',
  `datumv` date NOT NULL DEFAULT '0000-00-00',
  `used` enum('1','0') NOT NULL DEFAULT '0' COMMENT 'Flag koji se koristi pri odabitu otpremnica i ponuda na osnovu kojih se radi racun. 0-nije u racunu, 1 u racunu je. ',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vrac1opis`
--

DROP TABLE IF EXISTS `vrac1opis`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac1opis` (
  `rid` int(10) unsigned NOT NULL,
  `opis` varchar(1024) NOT NULL,
  PRIMARY KEY (`rid`),
  UNIQUE KEY `rid_UNIQUE` (`rid`),
  KEY `fk_vrac1opis_vrac1_idx_idx` (`rid`),
  CONSTRAINT `fk_vrac1opis_vrac1_idx` FOREIGN KEY (`rid`) REFERENCES `vrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vrac1porezi`
--

DROP TABLE IF EXISTS `vrac1porezi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac1porezi` (
  `rid` int(10) unsigned NOT NULL,
  `pid` int(10) unsigned NOT NULL,
  `porez_sifra` char(5) NOT NULL,
  `iznos_poreza` double(14,4) NOT NULL,
  `porez_postotak` double(14,7) NOT NULL DEFAULT '0.0000000',
  `osnovica` double(18,6) DEFAULT '0.000000',
  PRIMARY KEY (`rid`,`pid`),
  KEY `fk_vrac1porezivrac1ID_idx` (`rid`),
  CONSTRAINT `fk_vrac1porezivrac1ID` FOREIGN KEY (`rid`) REFERENCES `vrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vrac1vrac1conn`
--

DROP TABLE IF EXISTS `vrac1vrac1conn`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac1vrac1conn` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `vrac1racID` int(10) unsigned NOT NULL COMMENT 'ID racuna',
  `vrac1vezID` int(10) unsigned NOT NULL COMMENT 'ID otpremnice ili ponude na koju je racun vezan',
  `storno` enum('1','0') NOT NULL DEFAULT '0',
  `tip` enum('MO','MP','VO','VP','VA') NOT NULL DEFAULT 'VO' COMMENT 'MO-mracunotpremnica MP-mracunponuda VO-vracunotpremnica VP-vracunponuda VA-vracunpredujam',
  `iznos` double(14,4) NOT NULL DEFAULT '0.0000',
  `pdv` double(14,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `idx_vrac1racID` (`vrac1racID`),
  KEY `idx_vrac1vezID` (`vrac1vezID`),
  KEY `idx_vrac1storno` (`storno`),
  KEY `fk_vrac1vrac1conn_idx` (`vrac1racID`),
  CONSTRAINT `fk_vrac1vrac1connRacID` FOREIGN KEY (`vrac1racID`) REFERENCES `vrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vrac2`
--

DROP TABLE IF EXISTS `vrac2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac2` (
  `rid` int(10) unsigned NOT NULL,
  `aid` int(10) unsigned NOT NULL,
  `artsif` varchar(30) DEFAULT '',
  `jmjsifra` char(5) NOT NULL DEFAULT 'KOM',
  `kol` double(14,4) NOT NULL,
  `nc` double(14,4) DEFAULT NULL,
  `pcbpdv` double(14,4) DEFAULT NULL,
  `pc` double(14,4) DEFAULT NULL,
  `ukupno` double(14,4) DEFAULT NULL,
  `bpdv` double(14,4) DEFAULT NULL,
  `rabatP` double(14,4) DEFAULT NULL,
  `rabatK` double(14,4) DEFAULT NULL,
  `ukpcisti` double(14,4) DEFAULT NULL,
  `naziv` varchar(200) DEFAULT NULL,
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `povratna` double(14,4) DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  KEY `fk_vrac2_vrac1_idx` (`rid`),
  KEY `fk_vrac2artID_idx` (`aid`),
  CONSTRAINT `fk_vrac2artID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_vrac2vrac1ID` FOREIGN KEY (`rid`) REFERENCES `vrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `trigg_vrac2_normativ`
AFTER INSERT ON `vrac2`
FOR EACH ROW
BEGIN
	DECLARE _vrsta CHAR;
	SELECT vrsta INTO _vrsta from vrac1 WHERE id = NEW.rid;
	IF (_vrsta='M' OR _vrsta='V' OR _vrsta='O') THEN 
		call proc_vrac3_insert_normativ(NEW.rid,NEW.aid,NEW.aid,NEW.kol);
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `vrac2opis`
--

DROP TABLE IF EXISTS `vrac2opis`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac2opis` (
  `r2id` int(10) unsigned NOT NULL,
  `opis` text NOT NULL,
  PRIMARY KEY (`r2id`),
  UNIQUE KEY `r2id_UNIQUE` (`r2id`),
  KEY `fk_vrac2opis_vrac2_idx_idx` (`r2id`),
  CONSTRAINT `fk_vrac2opis_vrac2_idx` FOREIGN KEY (`r2id`) REFERENCES `vrac2` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vrac2porezi`
--

DROP TABLE IF EXISTS `vrac2porezi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac2porezi` (
  `rid` int(10) unsigned NOT NULL,
  `aid` int(10) unsigned NOT NULL,
  `pid` int(10) unsigned NOT NULL,
  `koeficijent_poreza` double(14,6) NOT NULL,
  `iznos_poreza` double(14,6) NOT NULL,
  `porez_opis` char(5) NOT NULL,
  `porez_postotak` double(14,7) NOT NULL DEFAULT '0.0000000',
  `r2ID` int(10) unsigned NOT NULL DEFAULT '0',
  `osnovica` double(14,6) NOT NULL DEFAULT '0.000000',
  KEY `fk_vrac2porezivrac1ID_idx` (`rid`),
  CONSTRAINT `fk_vrac2porezivrac1ID` FOREIGN KEY (`rid`) REFERENCES `vrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vrac3`
--

DROP TABLE IF EXISTS `vrac3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vrac3` (
  `rid` int(10) unsigned NOT NULL,
  `aid` int(10) unsigned NOT NULL,
  `nid` int(10) unsigned NOT NULL,
  `kol` double(14,4) NOT NULL,
  KEY `fk_vrac3vrac1ID_idx` (`rid`),
  KEY `fk_vrac3artID_idx` (`aid`),
  CONSTRAINT `fk_vrac3artID` FOREIGN KEY (`aid`) REFERENCES `art` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_vrac3vrac1ID` FOREIGN KEY (`rid`) REFERENCES `vrac1` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `vrac3_sklad`
AFTER INSERT ON `vrac3`
FOR EACH ROW
BEGIN
	DECLARE _Sklad int;
	DECLARE _Artsif varchar(30);
	DECLARE _ArtUsluga char;
	SET _ArtUsluga=(SELECT usluga FROM art WHERE id=NEW.nid);
	IF (_ArtUsluga='N') THEN
		set _Artsif=(select artsif from art where id=NEW.nid);
		set _Sklad=(select sid from vrac1 where id=NEW.rid);
		call proc_Update_Skladista(NEW.nid,_Sklad,(NEW.kol*-1),_Artsif);
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `ztotal`
--

DROP TABLE IF EXISTS `ztotal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ztotal` (
  `id` int(9) NOT NULL AUTO_INCREMENT,
  `datum` date DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping routines for database 'kasasrce'
--
/*!50003 DROP PROCEDURE IF EXISTS `ArtPoCijeni` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ArtPoCijeni`(
 IN _DatumOD datetime,
 IN _DatumDO datetime,
 IN _KasaID int(10))
begin
drop table if exists z_ArtPoCijeni;
create table z_ArtPoCijeni select r1.kasaid,r2.naziv,sum(r2.kol) as kolicina,r2.pc,sum(r2.ukupno) as ukupno 
  from rac2 r2 left join rac1 r1 on r2.rid=r1.id where r1.kasaid=_KasaID and r1.datumr between _DatumOD and _DatumDO
  group by r1.kasaid,r2.aid,r2.pc order by r2.naziv;
select * from z_ArtPoCijeni;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `iZakljSklad` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `iZakljSklad`(
 IN _DatumOD datetime,
 IN _DatumDO datetime,
 IN _SKLAD int(10))
begin
drop table if exists izPrometZakljDan;
create table izPrometZakljDan (
 aid int(10),
 naziv varchar(100),
 ulazPrimkaDo double(14,4) default 0,
 ulazMSKDo double(14,4) default 0,
 izlazMSKDo double(14,4) default 0,
 prodanoDo double(14,4) default 0,
 ulazPrimka double(14,4) default 0,
 ulazMSK double(14,4) default 0,
 izlazMSK double(14,4) default 0,
 prodano double(14,4) default 0,
 stanje double(14,4) default 0);
truncate izPrometZakljDan;
insert into izPrometZakljDan (aid,naziv) select id,naziv from art;  
update izPrometZakljDan set ulazPrimkaDo=0,ulazMskDo=0,izlazMSKDo=0,prodanoDo=0,ulazPrimka=0,ulazMSK=0,izlazMSK=0
,prodano=0,stanje=0;

update izPrometZakljDan i set i.ulazPrimkaDo=ifnull((select sum(p2.kolicina) from primk2 p2 left join primk1 p1 
on p2.dokid=p1.id where p1.skladid=_SKLAD and p1.datumdok < date(_DatumOD) and p2.aid=i.aid 
group by p2.aid),0);
update izPrometZakljDan i set i.ulazMSKdo=ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on
m2.mskid=m1.id where m1.datum < _DatumOD and m1.SIDna=_SKLAD and m2.aid=i.aid group by m2.aid),0);
update izPrometZakljDan i set i.izlazMSKDo=ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on 
 m2.mskid=m1.id where m1.datum < _DatumOD and m1.SIDsa=_SKLAD and m2.aid=i.aid group by m2.aid),0);
update izPrometZakljDan i set i.prodanoDo=ifnull((select sum(r3.kol) from rac3 r3 left join 
rac1 r1 on r3.rid=r1.id where r1.sid=_SKLAD and date(r1.datumr) < date(_DatumDO) and r3.nid=i.aid group by r3.nid),0);
update izPrometZakljDan i set i.ulazPrimka=ifnull((select sum(p2.kolicina) from primk2 p2 left join primk1 p1
on p2.dokid=p1.id where p1.skladid=_SKLAD and p1.datumdok between _DatumOD and _DatumDO and p2.aid=i.aid group by p2.aid),0);
update izPrometZakljDan i set i.ulazMSK=ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on
 m2.mskid=m1.id where m1.SIDna=_SKLAD and m1.datum between date(_DatumOD) and _DatumDO and m2.aid=i.aid group by m2.aid),0);
update izPrometZakljDan i set i.izlazMSK=ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on
m2.mskid=m1.id where m1.SIDsa=_SKLAD and m1.datum between date(_DatumOD) and _DatumDO and m2.aid=i.aid group by m2.aid),0);
update izPrometZakljDan i set i.prodano=ifnull((select sum(r3.kol) from rac3 r3 left join rac1 r1 on r3.rid=r1.id
 where r1.sid=_SKLAD and r1.datumr between _DatumOD and _DatumDO and r3.nid=i.aid group by r3.nid),0);
update izPrometZakljDan set stanje=ulazPrimkaDo+ulazMSKdo-izlazMSKDo-prodanoDo+ulazPrimka+ulazMSK-izlazMSK-prodano;
delete from izPrometZakljDan where ulazPrimkaDo=0 and ulazMskDo=0 and izlazMSKDo=0 and prodanoDo=0 and ulazPrimka=0
 and ulazMSK=0 and izlazMSK=0 and prodano=0 and stanje=0;
select * from izPrometZakljDan;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `izvPrometOdDo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `izvPrometOdDo`(
 IN _DatumOD date,
 IN _DatumDO date)
begin
 create temporary table iz1 
 select date(datumr) datum,sum(sum) suma from rac1 
 where date(datumr) between _DatumOD and _DatumDO group by date(datumr);
 select * from iz1;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `izvSankLista` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `izvSankLista`(
 IN _SKLAD int(10))
begin
drop table if exists izvSankLista;
create temporary table izvSankLista (
 aid int(10),
 stanje double(14,4) default 0
);
truncate skladnadan;
insert into skladnadan (aid) select id from art;
update skladnadan s set s.ulaz=(select sum(p2.kolicina) from primk2 p2 left join primk1 p1 
on p2.dokid=p1.id where p1.skladid=_SKLAD and aid=s.aid group by aid);
update skladnadan set ulaz=0 where ulaz is null;
update skladnadan s set s.izlaz=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on r3.rid=r1.id
where r1.sid=_SKLAD and nid=s.aid group by nid);
update skladnadan s set s.ulaz=s.ulaz+ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on
 m2.mskid=m1.id where m1.SIDna=_SKLAD and m2.aid=s.aid group by m2.aid),0);
update skladnadan set izlaz=0 where izlaz is null;
update skladnadan s set s.izlaz=s.izlaz+ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on
 m2.mskid=m1.id where m1.SIDsa=_SKLAD and m2.aid=s.aid group by m2.aid),0);
update skladnadan set ulaz=0 where ulaz is null;
update skladnadan set izlaz=0 where izlaz is null;
update skladnadan set stanje=ulaz-izlaz;
delete from skladnadan where ulaz=0 and izlaz=0 and stanje=0;


insert into izvSankLista (aid,stanje) select aid,stanje from skladnadan;
select a.naziv,i.stanje from izvSankLista i left join art a on i.aid=a.id order by a.poredak;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `izvZakljDan` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `izvZakljDan`(
 IN _DatumZS date)
begin
	drop table if exists izPrometZakljDan;
	create temporary table izPrometZakljDan (
	 aid int(10),
	 naziv varchar(100),
	 zaliha double(14,4) default 0,
	 primljeno double(14,4) default 0,
	 prodano double(14,4) default 0,
	 ostatak double(14,4) default 0,
	 cijena double(14,4) default 0,
	 iznos double(14,4) default 0);
	truncate skladnadan;
	insert into skladnadan (aid) select id from art;  
	update skladnadan s set s.ulaz=(select sum(p2.kolicina) from primk2 p2 left join primk1 p1 on p2.dokid=p1.id  
	where date(p1.datumdok) between '2013-01-01' and date(date_sub(_DatumZS,interval 1 day)) and aid=s.aid group by aid);
	update skladnadan s set s.izlaz=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on r3.rid=r1.id
	where date(r1.datumr) between '2013-01-01' and date(date_sub(_DatumZS,interval 1 day)) and nid=s.aid group by nid);
	update skladnadan set ulaz=0 where ulaz is null;update skladnadan set izlaz=0 where izlaz is null;
	update skladnadan set stanje = ulaz-izlaz;
	insert into izPrometZakljDan (aid,zaliha) select aid,stanje from skladnadan;
	update izPrometZakljDan i set i.primljeno=(select sum(p2.kolicina) from primk2 p2 left join
	 primk1 p1 on p2.dokid=p1.id where date(p1.datumdok)=date(_DatumZS) and p2.aid=i.aid group by aid);
	update izPrometZakljDan i set i.prodano=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on 
	 r3.rid=r1.id where date(r1.datumr)=date(_DatumZS) and r3.nid=i.aid group by r3.nid);
	update izPrometZakljDan set ostatak=0 where ostatak is null;
	update izPrometZakljDan set prodano=0 where prodano is null;
	update izPrometZakljDan set primljeno=0 where primljeno is null;
	update izPrometZakljDan set ostatak=zaliha+primljeno-prodano;
	delete from izPrometZakljDan where zaliha=0 and primljeno=0 and prodano=0 and ostatak=0;
	update izPrometZakljDan i,art a set i.naziv=a.naziv where i.aid=a.id;
	update izPrometZakljDan i,cjenikstavke c set i.cijena=c.punacijena where i.aid=c.aid and c.cid=3;
	update izPrometZakljDan set iznos=prodano*cijena;
	select * from izPrometZakljDan;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `izvZakljDanPoSKL` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `izvZakljDanPoSKL`(
 IN _DatumZS datetime,
 IN _DatumOD datetime,
 IN _DatumDO datetime,
 IN _SKLAD int(10))
begin
drop table if exists izPrometZakljDan;
create temporary table izPrometZakljDan (
 aid int(10),
 naziv varchar(100),
 zaliha double(14,4) default 0,
 primljeno double(14,4) default 0,
 prodano double(14,4) default 0,
 ostatak double(14,4) default 0,
 cijena double(14,4) default 0,
 iznos double(14,4) default 0);
truncate skladnadan;
insert into skladnadan (aid) select id from art;  
update skladnadan s set s.ulaz=(select sum(p2.kolicina) from primk2 p2 left join primk1 p1 
on p2.dokid=p1.id where p1.skladid=_SKLAD and p1.datumdok < date_sub(_DatumZS,interval 1 day) and aid=s.aid 
group by aid);
update skladnadan s set s.izlaz=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on r3.rid=r1.id
where r1.sid=_SKLAD and r1.datumr < date_sub(_DatumZS,interval 1 day) and nid=s.aid group by nid);
update skladnadan set ulaz=0 where ulaz is null;
update skladnadan set izlaz=0 where izlaz is null;
update skladnadan set stanje = ulaz-izlaz;
insert into izPrometZakljDan (aid,zaliha) select aid,stanje from skladnadan;
update izPrometZakljDan i set i.primljeno=(select sum(p2.kolicina) from primk2 p2 left join
 primk1 p1 on p2.dokid=p1.id where p1.skladid=_SKLAD and p1.datumdok between _DatumOD and _DatumDO
 and p2.aid=i.aid group by aid);
update izPrometZakljDan i set i.prodano=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on 
 r3.rid=r1.id where r1.sid=_SKLAD and r1.datumr between _DatumOD and _DatumDO 
 and r3.nid=i.aid group by r3.nid);
update izPrometZakljDan i set i.primljeno=i.primljeno+(select sum(m2.kolicina) from msk2 m2
left join msk1 m1 on m2.mskid=m1.id where m1.datum between _DatumOD and _DatumDO and m2.aid=i.aid
 group by m2.aid);
update izPrometZakljDan set ostatak=0 where ostatak is null;
update izPrometZakljDan set prodano=0 where prodano is null;
update izPrometZakljDan set primljeno=0 where primljeno is null;
update izPrometZakljDan set ostatak=zaliha+primljeno-prodano;
delete from izPrometZakljDan where zaliha=0 and primljeno=0 and prodano=0 and ostatak=0;
update izPrometZakljDan i,art a set i.naziv=a.naziv where i.aid=a.id;
update izPrometZakljDan i,cjenikstavke c set i.cijena=c.punacijena where i.aid=c.aid and c.cid=3;
update izPrometZakljDan set iznos=prodano*cijena;
select * from izPrometZakljDan;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_ArtiklPorezUlaz_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_ArtiklPorezUlaz_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _porezid int(10) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM artiklporezulaz 
	WHERE
		id = _id ) THEN 
		UPDATE artiklporezulaz SET 
				aid = _aid,
				porezid = _porezid
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO artiklporezulaz( id, aid, porezid)
		VALUES (_id,_aid,_porezid);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_ArtiklPorezUlaz_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_ArtiklPorezUlaz_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM artiklporezulaz 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_ArtiklPorez_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_ArtiklPorez_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _porezid int(10) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM artiklporez 
	WHERE
		id = _id ) THEN 
		UPDATE artiklporez SET 
				aid = _aid,
				porezid = _porezid
		WHERE
			id = _id ;
                                SET _lastInsertID = _id;
	ELSE
		INSERT INTO artiklporez( id, aid, porezid)
		VALUES (_id,_aid,_porezid);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_ArtiklPorez_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_ArtiklPorez_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM artiklporez 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_ArtSlike_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_ArtSlike_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _naziv varchar(200),
	 IN _opis varchar(200),
	 IN _image blob,
	 IN _imageSize int(10) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM artslike 
	WHERE
		id = _id ) THEN 
		UPDATE artslike SET 
				naziv = _naziv,
				opis = _opis,
				imageSize = _imageSize,
				image = _image
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO artslike( id, naziv, opis, imageSize, image)
		VALUES (_id,_naziv,_opis,_imageSize,_image);
		
		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_ArtSlike_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_ArtSlike_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM artslike 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Art_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Art_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _artsif varchar(30),
	 IN _barcode varchar(30),
	 IN _jmjid int(10) unsigned,
	 IN _alias varchar(100),
	 IN _naziv varchar(200),
	 IN _normativ enum('N','Y'),
	 IN _grupaid int(10) unsigned,
	 IN _vidljiv enum('1','0'),
	 IN _poredak int(10) unsigned,
	 IN _statgrupaid int(10) unsigned,
	 IN _usluga enum('N','Y'),
	 IN _povratnanaknada double(14,4),
	 IN _tip int(10) unsigned,
	 IN _slikaid int(10) unsigned,
	 IN _povratnanaknadaulazna double(14,4) unsigned, 
	 IN _kolicinapokomadu double(14,4) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 
	IF EXISTS(SELECT * FROM art 
	WHERE
		id = _id ) THEN 
		UPDATE art SET 
				artsif = _artsif,
				barcode = _barcode,
				jmjid = _jmjid,
				alias = _alias,
				naziv = _naziv,
				normativ = _normativ,
				grupaid = _grupaid,
				vidljiv = _vidljiv,
				poredak = _poredak,
				statgrupaid = _statgrupaid,
				usluga = _usluga,
				povratnanaknada = _povratnanaknada,
				tip = _tip,
				slikaid = _slikaid,
				povratnanaknadaulazna = _povratnanaknadaulazna,
				kolicinapokomadu = _kolicinapokomadu
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO art( id, artsif, barcode, jmjid, alias, naziv, normativ, grupaid, vidljiv, poredak, statgrupaid, usluga, povratnanaknada, tip, slikaid, povratnanaknadaulazna, kolicinapokomadu)
		VALUES (_id,_artsif,_barcode,_jmjid,_alias,_naziv,_normativ,_grupaid,_vidljiv,_poredak,_statgrupaid,_usluga,_povratnanaknada,_tip,_slikaid,_povratnanaknadaulazna,_kolicinapokomadu);
		
		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Art_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Art_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM art 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Art_GetNextFreeSifra` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Art_GetNextFreeSifra`(
	OUT _nextArtSifra  int(10) unsigned
)
BEGIN 
	SET _nextArtSifra = (SELECT ifnull(max( CONVERT(artsif, unsigned) ),0) FROM art WHERE (artsif REGEXP '^[0-9]+$')) + 1;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Art_SetDeleted` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Art_SetDeleted`(
         IN _id int(10) unsigned
)
BEGIN
        DELETE FROM artiklporez 
        WHERE
                aid = _id;
        DELETE FROM cjenikstavke
        WHERE
                aid = _id;
        DELETE FROM normativ 
        WHERE
                aid = _id OR nid = _id;
        UPDATE art SET 
                artsif =  CONCAT('_D', _id, ' ', artsif),
                deleted = '1'
        WHERE
                id = _id;
        DELETE FROM locrac2 
        WHERE
                aid = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_cant1` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_cant1`()
BEGIN
DECLARE newKolicina double(14,4);
DECLARE newartnorm_id int(10) unsigned;
DECLARE done INT DEFAULT FALSE;
DECLARE counter INT DEFAULT 0;

DECLARE cursorNorm CURSOR FOR 
SELECT id from art; 
DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

OPEN cursorNorm;

truncate art_normativ_table;
read_loop: LOOP
FETCH cursorNorm INTO newartnorm_id;
IF done THEN
LEAVE read_loop;
END IF;
SET counter = counter + 1;
SET newKolicina = 1;
CALL proc_cant2(newartnorm_id, newartnorm_id, newKolicina);

END LOOP;
CLOSE cursorNorm;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_cant2` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_cant2`(
IN _aid int(10) unsigned,
IN _nid int(10) unsigned,
IN _kolicina double(14,4)
)
BEGIN
DECLARE newKolicina double(14,4);
DECLARE newartnorm_id int(10) unsigned;
DECLARE done INT DEFAULT FALSE;
DECLARE counter INT DEFAULT 0;

DECLARE cursorNorm CURSOR FOR 
SELECT nid, kolicina FROM normativ 
WHERE aid = _nid;
DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

OPEN cursorNorm;

read_loop: LOOP
FETCH cursorNorm INTO newartnorm_id, newKolicina;
IF done THEN
LEAVE read_loop;
END IF;
SET counter = counter + 1;
SET newKolicina = newKolicina * _kolicina;
CALL proc_cant2(_aid, newartnorm_id, newKolicina);

END LOOP;
CLOSE cursorNorm;
IF counter = 0 THEN
INSERT INTO art_normativ_table( aid, nid, kol)
VALUES (_aid,_nid,_kolicina);
END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_CjenikSkladisteVeza_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_CjenikSkladisteVeza_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _sid int(10) unsigned,
	 IN _cid int(10) unsigned,
	 IN _isdefault enum('N','Y'),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 
	IF _isdefault = 'Y' THEN
		IF EXISTS(SELECT * FROM cjenikskladisteveza 
		WHERE
			_sid = sid 
			AND _cid <> cid 
			AND isdefault = 'Y' 
			AND _id <> id) THEN 
			UPDATE cjenikskladisteveza SET 
					isdefault = 'N'
			WHERE
				_sid = sid
				AND isdefault = 'Y';
		END IF;
	END IF;

	IF EXISTS(SELECT * FROM cjenikskladisteveza 
	WHERE
		id = _id ) THEN 
		UPDATE cjenikskladisteveza SET 
				sid = _sid,
				cid = _cid,
				isdefault = _isdefault
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO cjenikskladisteveza( id, sid, cid, isdefault)
		VALUES (_id,_sid,_cid,_isdefault);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_CjenikSkladisteVeza_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_CjenikSkladisteVeza_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM cjenikskladisteveza 
	WHERE
			id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_CjenikStavke_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_CjenikStavke_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _cid int(10) unsigned,
	 IN _punacijena double(14,4) unsigned,
	 IN _veleprodajnacijena double(14,4) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM cjenikstavke 
	WHERE
		id = _id ) THEN 
		UPDATE cjenikstavke SET 
				aid = _aid,
				cid = _cid,
				punacijena = _punacijena,
				veleprodajnacijena = _veleprodajnacijena
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO cjenikstavke( id, aid, cid, punacijena, veleprodajnacijena)
		VALUES (_id,_aid,_cid,_punacijena,_veleprodajnacijena);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_CjenikStavke_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_CjenikStavke_Delete`(
     IN _id int(10) unsigned
)
BEGIN

    DELETE FROM cjenikstavke
    WHERE
            id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_CjenikTjedniSchedule_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_CjenikTjedniSchedule_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _cid int(10) unsigned,
	 IN _vrijediod time,
	 IN _vrijedido time,
	 IN _dayofweek int(10) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM cjeniktjednischedule 
	WHERE
		id = _id ) THEN 
		UPDATE cjeniktjednischedule SET 
				cid = _cid,
				vrijediod = _vrijediod,
				vrijedido = _vrijedido,
				dayofweek = _dayofweek
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO cjeniktjednischedule( id, cid, vrijediod, vrijedido, dayofweek)
		VALUES (_id,_cid,_vrijediod,_vrijedido,_dayofweek);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_CjenikTjedniSchedule_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_CjenikTjedniSchedule_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM cjeniktjednischedule 
	WHERE
		id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Cjenik_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Cjenik_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _sifra char(10),
	 IN _naziv varchar(50),
	 IN _opis varchar(250),
	 IN _glavni enum('N','Y'),
	 IN _aktivan enum('N','Y'),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM cjenik 
	WHERE
		id = _id ) THEN 
		UPDATE cjenik SET 
				sifra = _sifra,
				naziv = _naziv,
				opis = _opis,
				glavni = _glavni,
				aktivan = _aktivan
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO cjenik( id, sifra, naziv, opis, glavni, aktivan)
		VALUES (_id,_sifra,_naziv,_opis,_glavni,_aktivan);
		
		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Cjenik_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Cjenik_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM cjenik 
	WHERE
			id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Cjenik_GetIDActualNow` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Cjenik_GetIDActualNow`(
IN _sid int(10) unsigned,
OUT _cID  int(10) unsigned
)
BEGIN 

IF EXISTS(SELECT c.id
FROM cjenikskladisteveza cs, cjeniktjednischedule cts, cjenik c
WHERE cs.sid = _sid
AND cs.cid = cts.cid
AND c.id = cs.cid
AND c.aktivan = 'Y'
AND cs.isdefault = 'N'
AND cts.dayofweek = DAYOFWEEK (CURDATE())
AND cts.vrijediod <= CURTIME() AND cts.vrijedido >= CURTIME()
LIMIT 1) 
THEN 
SELECT c.id INTO _cID
FROM cjenikskladisteveza cs, cjeniktjednischedule cts, cjenik c
WHERE cs.sid = _sid
AND cs.cid = cts.cid
AND c.id = cs.cid
AND c.aktivan = 'Y'
AND cs.isdefault = 'N'
AND cts.dayofweek = DAYOFWEEK (CURDATE())
AND cts.vrijediod <= CURTIME() AND cts.vrijedido >= CURTIME()
LIMIT 1;
ELSE
SELECT c.id INTO _cID
FROM cjenikskladisteveza cs, cjenik c
WHERE cs.sid = _sid
AND c.id = cs.cid
AND c.aktivan = 'Y'
AND cs.isdefault = 'Y';
END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Company_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Company_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _naziv varchar(250),
	 IN _adresa varchar(250),
	 IN _mjesto varchar(100),
	 IN _dobavljac enum('Y','N'),
	 IN _oib char(20),
	 IN _MB char(13),
	 IN _sifra int(10) unsigned,
	 IN _rabat double(4,2),
	 IN _opis longtext,
	 IN _countrycode char(2),
	 IN _email varchar(250),
	 IN _telefon varchar(250),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM company
	WHERE
		id = _id ) THEN 
		UPDATE company SET 
				naziv = _naziv,
				adresa = _adresa,
				mjesto = _mjesto,
				dobavljac = _dobavljac,
				oib = _oib,
				MB = _MB,
				sifra = _sifra,
				rabat = _rabat,
				opis = _opis,
				countrycode = _countrycode,
				email = _email,
				telefon = _telefon
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO company( id, naziv, adresa, dobavljac, oib, MB, sifra, rabat, opis, mjesto, countrycode, email, telefon)
		VALUES (_id,_naziv,_adresa,_dobavljac,_oib,_MB,_sifra,_rabat,_opis,_mjesto,_countrycode,_email,_telefon);
		
		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Company_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Company_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM company 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_DohvatBrojaDokumenta` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_DohvatBrojaDokumenta`(
	 IN _vrsta enum('M','V','O','P','RN','A','PR'),
	 IN _kasaid int(10),
	 IN _oznpp varchar(20),
	 IN _oznsljed ENUM('N','P'),
	 IN _iddoc int(10) unsigned,
	OUT _NoviBroj int(10)
)
BEGIN 
	DECLARE _lastInsertID int(10) unsigned;
	DECLARE _vrstaInterno char(4);
	DECLARE _startBrojDefined int(10);
	
	IF _vrsta IN ('M','V','A') THEN
		SET _vrstaInterno = 'R';
	ELSE
		SET _vrstaInterno = _vrsta;
	END IF;

	
	IF _oznsljed = 'N' THEN
		SET _startBrojDefined = (SELECT ifnull(max(br), 1) FROM brojracuna_start WHERE vrsta = _vrstaInterno AND kasaid = _kasaid AND oznpp = _oznpp AND godina = YEAR(NOW()));
	ELSE
		SET _startBrojDefined = (SELECT ifnull(max(br), 1) FROM brojracuna_start WHERE vrsta = _vrstaInterno AND oznpp = _oznpp AND godina = YEAR(NOW()));
	END IF;

	
	IF _oznsljed = 'N' THEN
		INSERT INTO brojracuna(br,godina,vrsta,kasaid,oznpp,oznsljed,datum,vrstareal,iddoc)
		SELECT ifnull(max(br)+1,_startBrojDefined),YEAR(NOW()),_vrstaInterno,_kasaid,_oznpp,_oznsljed,NOW(),_vrsta,_iddoc
		FROM brojracuna WHERE vrsta = _vrstaInterno AND kasaid = _kasaid AND oznpp = _oznpp AND godina = YEAR(NOW());
	ELSE
		INSERT INTO brojracuna(br,godina,vrsta,kasaid,oznpp,oznsljed,datum,vrstareal,iddoc)
		SELECT ifnull(max(br)+1,_startBrojDefined),YEAR(NOW()),_vrstaInterno,_kasaid,_oznpp,_oznsljed,NOW(),_vrsta,_iddoc
		FROM brojracuna WHERE vrsta = _vrstaInterno AND oznpp = _oznpp AND godina = YEAR(NOW());
	END IF;
	SET _lastInsertID = LAST_INSERT_ID(); 

	
	SET _NoviBroj=(SELECT br FROM brojracuna WHERE id = _lastInsertID);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_GrupaPorezUpdateArtiklPorezPoGrupi` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_GrupaPorezUpdateArtiklPorezPoGrupi`(
	 IN _grupaid int(10) unsigned
)
BEGIN
	DECLARE _porezid int(10) unsigned;
	DECLARE done INT DEFAULT FALSE;
	
	DECLARE cursorP CURSOR FOR 
		SELECT porezid FROM grupeporez 
		WHERE gid = _grupaid;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	
	DELETE FROM artiklporez 
	WHERE id IN (SELECT id FROM(
		SELECT ap.id FROM
		artiklporez ap, art a
		WHERE ap.aid = a.id
		AND a.grupaid = _grupaid
		AND ap.porezid NOT IN (
			SELECT gp.porezid
			FROM grupe g, grupeporez gp
			WHERE g.id = gp.gid
			AND g.ID = _grupaid)
	) as tmptable);

	
	OPEN cursorP;

read_loop: LOOP
	FETCH cursorP INTO _porezid;
	IF done THEN
		LEAVE read_loop;
    END IF;

	INSERT INTO artiklporez (aid, porezid)
	(SELECT a.id, _porezid
	FROM art a
	WHERE a.grupaID = _grupaid
	and a.id NOT IN (
		SELECT ap.aid
		FROM artiklporez ap
		WHERE ap.porezID = _porezid
	));

	END LOOP;
	CLOSE cursorP;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_GrupaPorezUpdateArtiklPorezZaArtikl` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_GrupaPorezUpdateArtiklPorezZaArtikl`(
	 IN _artid int(10) unsigned
)
BEGIN
	DECLARE _porezid int(10) unsigned;
	DECLARE _grupaid int(10) unsigned;
	DECLARE done INT DEFAULT FALSE;
		
	DECLARE cursorP CURSOR FOR 
		SELECT porezid FROM grupeporez 
		WHERE gid = _grupaid;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	
	SELECT grupaid INTO _grupaid
	FROM art
	WHERE id = _artid;

	
	DELETE FROM artiklporez 
	WHERE id IN (SELECT id FROM(
		SELECT ap.id FROM
		artiklporez ap, art a
		WHERE ap.aid = a.id
		AND a.id = _artid
		AND a.grupaid = _grupaid
		AND ap.porezid NOT IN (
			SELECT gp.porezid
			FROM grupe g, grupeporez gp
			WHERE g.id = gp.gid
			AND g.ID = _grupaid)
	) as tmptable);

	
	OPEN cursorP;

read_loop: LOOP
	FETCH cursorP INTO _porezid;
	IF done THEN
		LEAVE read_loop;
    END IF;

	INSERT INTO artiklporez (aid, porezid)
	(SELECT a.id, _porezid
	FROM art a
	WHERE a.grupaID = _grupaid
	AND a.id = _artid
	and a.id NOT IN (
		SELECT ap.aid
		FROM artiklporez ap
		WHERE ap.porezID = _porezid
	));

	END LOOP;
	CLOSE cursorP;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_GrupePorez_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_GrupePorez_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _gid int(10) unsigned,
	 IN _porezid int(10) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM grupeporez 
	WHERE
		id = _id ) THEN 
		UPDATE grupeporez SET 
				gid = _gid,
				porezid = _porezid
		WHERE
			id = _id ;
        SET _lastInsertID = _id;
	ELSE
		INSERT INTO grupeporez( id, gid, porezid)
		VALUES (_id,_gid,_porezid);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_GrupePorez_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_GrupePorez_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM grupeporez 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Grupe_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Grupe_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _opis varchar(250),
	 IN _glavna tinyint(1),
	 IN _ispis_kuhinja tinyint(1),
	 IN _statisticka enum('N','Y'),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM grupe 
	WHERE
		id = _id ) THEN 
		UPDATE grupe SET 
				opis = _opis,
				glavna = _glavna,
				ispis_kuhinja = _ispis_kuhinja,
				statisticka = _statisticka
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO grupe( id, opis, glavna, ispis_kuhinja, statisticka)
		VALUES (_id,_opis,_glavna, _ispis_kuhinja, _statisticka);
		
		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Grupe_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Grupe_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM grupe 
	WHERE
		id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_IzvInv` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_IzvInv`(
    IN _SID int(10),
    IN _DatOd datetime,
    IN _DatDo datetime
)
BEGIN
truncate izvinv;
truncate serbr;
insert into serbr select 0,'primk','U','Y',p1.datumdok,p1.id,p1.skladid,p2.aid,p2.kolicina,p2.fcijenajedinicna   from primk1 p1 left join primk2 p2 on p1.id=p2.dokid;
insert into serbr select 0,'rac3','I','Y',r.datumr,r.id,r.sid,r3.nid,r3.kol*-1,0 from rac1 r left join rac3 r3 on r.id=r3.rid;
insert into izvinv (aid) select id from art;
update izvinv i,(select aid,sum(kol) k from serbr where sid=_SID group by aid) s set i.stanje=s.k where i.aid=s.aid;
update izvinv i,(select aid,sum(kol) k from serbr where sid=_SID and ulazizlaz='U' group by aid) s set i.ulaz=s.k where i.aid=s.aid;
update izvinv i,(select aid,sum(kol)*-1 k from serbr where sid=_SID and ulazizlaz='I' group by aid) s set i.izlaz=s.k where i.aid=s.aid;
update izvinv i,(select aid,sum(kol) k from serbr where sid=_SID and ulazizlaz='U' and datum between _DatOd and _DatDo group by aid) s set i.ulaza_od=s.k where i.aid=s.aid;
update izvinv i,(select aid,sum(kol)*-1 k from serbr where sid=_SID and ulazizlaz='I' and datum between _DatOd and _DatDo group by aid) s set i.izlaza_od=s.k where i.aid=s.aid;
update izvinv set stanjeod=ulaza_od-izlaza_od;
update izvinv i,(select aid,sum(kol) k from serbr where sid=_SID and datum <= _DatDo group by aid) s set i.stanje_datod=s.k where i.aid=s.aid;
delete from izvinv where stanje=0 and ulaz=0 and izlaz=0 and ulaza_od=0 and izlaza_od=0;
select a.naziv,i.* from izvinv i left join art a on i.aid=a.id order by a.naziv;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Izvod_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Izvod_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _vrid int(10) unsigned,
	 IN _datum_placanja date,
	 IN _iznos double(14,2),
	 IN _opis_placanja varchar(200),
	 IN _poziv varchar(200),
	 IN _izvodbr int(10) unsigned,
	 IN _izvodgod int(10) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM izvod 
	WHERE
		id = _id ) THEN 
		UPDATE izvod SET 
			vrid = _vrid,
			datum_placanja = _datum_placanja,
			iznos = _iznos,
			opis_placanja = _opis_placanja,
			poziv = _poziv,
			izvodbr = _izvodbr,
			izvodgod = _izvodgod
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO izvod( id, vrid, datum_placanja, iznos, opis_placanja, poziv, izvodbr, izvodgod)
		VALUES (_id,_vrid,_datum_placanja,_iznos,_opis_placanja,_poziv,_izvodbr,_izvodgod);
		
		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Izvod_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Izvod_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM izvod 
	WHERE
		id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_izvPoKonobarimaPromet` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_izvPoKonobarimaPromet`( IN _DatumOd datetime,IN _DateDo datetime)
BEGIN
update rac1 r1,narudzba1 n set r1.narucio_uid=n.narucio_uid where r1.narid=n.id and r1.narucio_uid=0;
update rac1 set narucio_uid=0 where narucio_uid is null;
select 'glavni',kor.puninaziv,k.naziv as nacin_placanja,sum(sum) as suma,kod.value,(sum(sum)*kod.value/100) dodatno, r1.kasaid from rac1 r1 left join kartice k on r1.np=k.id left join korisnik kor on r1.uid=kor.id left join korisnik_dodatno kod on kor.id=kod.uid  where r1.narid=0 and datumr between _DatumOd and _DateDo group by r1.uid,k.id union all 
select 'vanjski',kor.puninaziv,k.naziv as nacin_placanja,sum(sum) as suma,kod.value,(sum(sum)*kod.value/100) dodatno, r1.kasaid from rac1 r1 left join kartice k on r1.np=k.id left join korisnik kor on r1.narucio_uid=kor.id left join korisnik_dodatno kod on kor.id=kod.uid  where r1.narid>0 and datumr between _DatumOd and _DateDo group by r1.narucio_uid,k.id;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_izvSankLista` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_izvSankLista`(
 IN _SKLAD int(10))
begin
drop table if exists izvSankLista;
create temporary table izvSankLista (
 aid int(10),
 stanje double(14,4) default 0
);
truncate skladnadan;
insert into skladnadan (aid) select id from art;  
update skladnadan s set s.ulaz=(select sum(p2.kolicina) from primk2 p2 left join primk1 p1 
on p2.dokid=p1.id where p1.skladid=_SKLAD and aid=s.aid group by aid);
update skladnadan s set s.izlaz=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on r3.rid=r1.id
where r1.sid=_SKLAD and nid=s.aid group by nid);
update skladnadan s set s.ulaz=s.ulaz+ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on
 m2.mskid=m1.id where m1.SIDna=_SKLAD and m2.aid=s.aid group by m2.aid),0);
update skladnadan s set s.izlaz=s.izlaz-ifnull((select sum(m2.kolicina) from msk2 m2 left join msk1 m1 on
 m2.mskid=m1.id where m1.SIDsa=_SKLAD and m2.aid=s.aid group by m2.aid),0);
update skladnadan set ulaz=0 where ulaz is null;
update skladnadan set izlaz=0 where izlaz is null;
update skladnadan set stanje = ulaz-izlaz;
delete from skladnadan where ulaz=0 and izlaz=0 and stanje=0;
insert into izvSankLista (aid,stanje) select aid,stanje from skladnadan;
select i.aid,a.naziv,i.stanje from izvSankLista i left join art a on i.aid=a.id order by a.poredak;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Izv_PoArtiklima` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Izv_PoArtiklima`(
	_DatumOd date,
	_DatumDo date
)
begin
	IF (_DatumOd = "0000-00-00") then 
	  set _DatumOd = "2013-01-01";
    end IF;
    IF (_DatumDo = "0000-00-00") then 
      set _DatumDo = date(now()); 
	end IF;
    drop table if exists izvPoArtiklima;
	create temporary table izvPoArtiklima 
	select r2.aid,r2.naziv,sum(r2.kol) kolicina,r2.pc pc,sum(r2.ukupno) suma,
	r2p.porez_opis,sum(r2p.iznos_poreza) izn_poreza,date(r1.datumr) datum,
	r1.vrsta vrsta,r1.kasaid 
	from rac1 r1 left join rac2 r2 on r1.id=r2.rid left join rac2porezi r2p on (r2.rid=r2p.rid and r2.aid=r2p.aid) 
	where date(r1.datumr) between _DatumOd and _DatumDo 
	group by date(r1.datumr),r2.aid,r2.pc,r2p.porez_opis order by datum,porez_opis;
	select * from izvPoArtiklima;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Izv_PoRobi` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Izv_PoRobi`(
	_DatumOd date,
	_DatumDo date
)
begin
	IF (_DatumOd = "0000-00-00") then 
	  set _DatumOd = "2013-01-01";
    end IF;
    IF (_DatumDo = "0000-00-00") then 
      set _DatumDo = date(now()); 
	end IF;
    drop table if exists izvPoRobi;
	create temporary table izvPoRobi 
    select r3.nid,a.naziv,sum(r3.kol) kolicina,r2p.porez_opis,sum(r2p.iznos_poreza) iznos_poreza,
    date(r1.datumr) datum,r1.vrsta,r1.kasaid from rac1 r1 left join rac3 r3 on r1.id=r3.rid 
    left join rac2porezi r2p on (r3.rid=r2p.rid and r3.aid=r2p.aid) left join art a on r3.nid=a.id 
	where date(r1.datumr) between _DatumOd and _DatumDo 
    group by r3.nid,r2p.porez_opis order by r1.datumr,r2p.porez_opis;
	select * from izvPoRobi;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_izv_SkladNaDan` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_izv_SkladNaDan`(
 in _DatumOD date,
 in _DatumDO date)
begin
	
	truncate skladnadan;

	
	INSERT INTO skladnadan (aid, ulaz, izlaz, stanje) 
	SELECT id, 0, 0, 0 FROM art
	WHERE usluga = 'N';

	
	update skladnadan s set s.ulaz=(select sum(p2.kolicina) from primk2 p2 left join primk1 p1 on p2.dokid=p1.id 
	where date(p1.datumdok) between _DatumOD and _DatumDO and aid=s.aid group by aid);

	
	update skladnadan s set s.izlaz=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on r3.rid=r1.id 
	where date(r1.datumr) between _DatumOD and _DatumDO and nid=s.aid group by nid);

	
	update skladnadan set ulaz=0 where ulaz is null;
	update skladnadan set izlaz=0 where izlaz is null;

	
	update skladnadan s set s.izlaz = s.izlaz + 
	ifnull((
		select sum(r3.kol) from vrac3 r3 left join vrac1 r1 on r3.rid=r1.id 
		where date(r1.datumr) between _DatumOD and _DatumDO and r3.nid=s.aid 
		and r1.vrsta in ('M', 'V', 'O')
		group by nid
	),0);
   
   update skladnadan s set s.izlaz = s.izlaz +
   ifnull((
      select sum(rn3.kol) from rnalog3 rn3 left join rnalog1 rn1 on rn3.rid=rn1.id
      where rn3.skinuto='1' and date(rn1.datumz) between _DatumOD and _DatumDO and rn3.aid=s.aid
      group by aid
   ),0);
	
	update skladnadan set ulaz=0 where ulaz is null;
	update skladnadan set izlaz=0 where izlaz is null;
	
	update skladnadan set stanje = ulaz-izlaz;
	
	delete from skladnadan where stanje=0 and ulaz=0 and izlaz=0;

	
    select a.artsif,a.naziv,s.stanje,s.ulaz,s.izlaz 
	from skladnadan s left join art a on s.aid=a.id 
	order by a.naziv;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_KnjigaPopisa` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_KnjigaPopisa`(
    IN _SkladID int(10),
    IN _DatumOd date,
    IN _DatumDo date,
    IN _RPT bool
)
BEGIN
DECLARE _PredZaduzenje double(14,4);
drop table if exists tmp_kp;

if (_RPT and (date_format(_DatumOd,'%m-%d')<>'01-01')) then
 call proc_KnjigaPopisa(_SkladID,concat(year(now()),'-01-01'),_DatumOd,false);
 set _PredZaduzenje = (select sum(zaduzenje)-sum(razduzenje) from tmp_kp);
end if;

drop table if exists tmp_kp;
create table tmp_kp (sklad varchar(250),datumdok date,opis varchar(250),zaduzenje double(14,2),razduzenje double(14,2));
if (_RPT and (date_format(_DatumOd,'%m-%d')<>'01-01')) then 
 insert into tmp_kp select (select concat(sifra,'-',naziv) from matsklad where id=_SkladID),_DatumOd,'Pocetno zaduzenje',_PredZaduzenje,0;
end if;

insert into tmp_kp select concat(s.sifra,'-',s.naziv) sklad,p1.datumdok,concat("KLK.",p1.dokbr,"-",c.naziv) opis,sum(((p2.kolicina*p2.pcjedinicna)*0.25)+(p2.kolicina*p2.pcjedinicna)) zaduzenje,0 razduzenje from primk1 p1 left join primk2 p2 on p1.id=p2.dokid left join company c on p1.dobavljacid=c.id left join matsklad s on p1.skladid=s.id where p1.skladid=_SkladID  and p1.datumdok between _DatumOd and _DatumDo group by p1.id,p1.datumdok;

insert into tmp_kp
select concat(s.sifra,'-',s.naziv) sklad,date(r1.datumr) datumdok,'Dnevna prodaja maloprodaja' opis,0 zaduzenje,convert(sum(r2.ukupno),decimal(14,2)) razduzenje from rac1 r1 left join matsklad s on r1.sid=s.id left join rac2 r2 on r1.id=r2.rid left join art a on r2.aid=a.id where a.normativ='Y' and r1.sid=_SkladID and date(r1.datumr) between _DatumOd and _DatumDo  group by date(r1.datumr) 
union all 
select concat(s.sifra,'-',s.naziv) sklad,date(r1.datumr) datumdok,
concat(if(r1.rabatp<>0,concat('RacUKP_Pop-',convert(r1.rabatp,decimal(14,2)),"% "),""),
concat("Art_Pop-",convert(r2.rabatp,decimal(14,2))),'% POP na robu - Racun br ',r1.br) opis,convert((sum((r1.rabatp*r2.ukupno)/100)+r2.rabatk)*-1,decimal(14,2)) zaduzenje,0 razduzenje from rac1 r1 left join matsklad s on r1.sid=s.id left join rac2 r2 on r1.id=r2.rid left join art a on r2.aid=a.id where a.normativ='Y' and r1.sid=_SkladID and date(r1.datumr) between _DatumOd and _DatumDo and (r1.rabatp<>0 or r2.rabatp<>0) group by date(r1.datumr);

insert into tmp_kp 
select concat(s.sifra,'-',s.naziv) sklad,date(r1.datumr) datumdok,'Dnevna prodaja veleprodaja' opis,0 zaduzenje,convert(sum(r2.ukupno),decimal(14,2)) razduzenje from vrac1 r1 left join matsklad s on r1.sid=s.id left join vrac2 r2 on r1.id=r2.rid left join art a on r2.aid=a.id where a.normativ='Y' and r1.sid=_SkladID and date(r1.datumr) between _DatumOd and _DatumDo  group by date(r1.datumr) 
union all 
select concat(s.sifra,'-',s.naziv) sklad,date(r1.datumr) datumdok,
concat(if(r1.rabatp<>0,concat('VRacUKP_Pop-',convert(r1.rabatp,decimal(14,2)),"% "),""),
concat("Art_Pop-",convert(r2.rabatp,decimal(14,2))),'% POP na robu - VRacun br ',r1.br) opis,convert((sum((r1.rabatp*r2.ukupno)/100)+r2.rabatk)*-1,decimal(14,2)) zaduzenje,0 razduzenje from vrac1 r1 left join matsklad s on r1.sid=s.id left join vrac2 r2 on r1.id=r2.rid left join art a on r2.aid=a.id where a.normativ='Y' and r1.sid=_SkladID and date(r1.datumr) between _DatumOd and _DatumDo and (r1.rabatp<>0 or r2.rabatp<>0) group by date(r1.datumr);


delete from tmp_kp where datumdok='0000-00-00';
if (_RPT) then
 set @rbr=0;select (@rbr :=@rbr+1) as rbr,k.* from tmp_kp k order by datumdok;
end if;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Korisnik_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Korisnik_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _naziv varchar(100),
	 IN _PuniNaziv varchar(250),
	 IN _pregled enum('N','Y'),
	 IN _sifra varchar(100),
	 IN _dozvole int(3),
	 IN _oib char(11),
	 IN _active enum('A','N','O'),
	 IN _rfID varchar(100),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM korisnik 
	WHERE
		id = _id ) THEN 
		UPDATE korisnik SET 
				naziv = _naziv,
				PuniNaziv = _PuniNaziv,
				pregled = _pregled,
				sifra = _sifra,
				dozvole = _dozvole,
				oib = _oib,
				active = _active,
				rfID = _rfID
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO korisnik( id, naziv, PuniNaziv, pregled, sifra, dozvole, oib, active, rfID)
		VALUES (_id,_naziv,_PuniNaziv,_pregled,_sifra,_dozvole,_oib,_active,_rfID);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Korisnik_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Korisnik_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM korisnik 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocPrimk1_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocPrimk1_AddUpdate`(
	 IN _id int(10) unsigned,
	OUT _dokbr int(10) unsigned,
	 IN _dobavljacID int(10) unsigned,
	 IN _datumdok date,
	 IN _datumv date,
	 IN _skladID int(10) unsigned,
	 IN _tipfID int(10) unsigned,
	 IN _vezadokument varchar(100),
	 IN _fiznosstart double(14,4),
	 IN _fiznosrabatkn double(14,4),
	 IN _fiznosukupno double(14,4),
	 IN _fiznospdv double(14,4),
	 IN _ztiznososn double(14,4),
	 IN _ztiznospdv double(14,4),
	 IN _ztkoefi double(14,4),
	 IN _iznosukupno double(14,4),
	 IN _valsifra char(3),
	 IN _valdatum date,
	 IN _valtecaj double(14,4),
	 IN _valiznos double(14,4),
	 IN _storno enum('0','1'),
	 IN _storno_datum date,
	 IN _kasaid int(10),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM locprimk1 
	WHERE
		id = _id ) THEN 
		UPDATE locprimk1 SET 
				dobavljacID = _dobavljacID,
				datumdok = _datumdok,
				datumv = _datumv,
				skladID = _skladID,
				tipfID = _tipfID,
				vezadokument = _vezadokument,
				fiznosstart = _fiznosstart,
				fiznosrabatkn = _fiznosrabatkn,
				fiznosukupno = _fiznosukupno,
				fiznospdv = _fiznospdv,
				ztiznososn = _ztiznososn,
				ztiznospdv = _ztiznospdv,
				ztkoefi = _ztkoefi,
				iznosukupno = _iznosukupno,
				valsifra = _valsifra,
				valdatum = _valdatum,
				valtecaj = _valtecaj,
				valiznos = _valiznos,
				storno = _storno,
				storno_datum = _storno_datum,
				kasaid = _kasaid
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO locprimk1( id, dokbr, dobavljacID, datumdok, datumv, skladID, tipfID, vezadokument, fiznosstart, fiznosrabatkn, fiznosukupno, fiznospdv, ztiznososn, ztiznospdv, ztkoefi, iznosukupno, valsifra, valdatum, valtecaj, valiznos, storno, storno_datum, kasaid)
		SELECT _id,ifnull(max(dokbr)+1,1),_dobavljacID,_datumdok,_datumv,_skladID,_tipfID,_vezadokument,_fiznosstart,_fiznosrabatkn,_fiznosukupno,_fiznospdv,_ztiznososn,_ztiznospdv,_ztkoefi,_iznosukupno,_valsifra,_valdatum,_valtecaj,_valiznos,_storno,_storno_datum,_kasaid
		FROM locprimk1;

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;

	SELECT dokbr INTO _dokbr FROM locprimk1
	WHERE id = _lastInsertID ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocPrimk1_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocPrimk1_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 
	DELETE FROM locprimk1 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocPrimk2_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocPrimk2_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _dokid int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _naziv varchar(250),
	 IN _kolicina double(14,4),
	 IN _jmj char(5),
	 IN _fcijenajedinicna double(14,4),
	 IN _fcijenaukupno double(14,4),
	 IN _frabatp double(14,4),
	 IN _frabatknjed double(14,4),
	 IN _frabatknukp double(14,4),
	 IN _fcijenaukupnoumanjeno double(14,4),
	 IN _ncukupnoitroskovi double(14,4),
	 IN _marzap double(14,4),
	 IN _marzakn double(14,4),
	 IN _ztdokiznososn double(14,4),
	 IN _ztdokiznospdv double(14,4),
	 IN _ztdokkoefi double(14,4),
	 IN _ncpdvukupno double(14,4),
	 IN _ncpdvodbitakp double(14,4),
	 IN _ncpdvodbitakkn double(14,4),
	 IN _pcjedinicna double(14,4),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM locprimk2 
	WHERE
		id = _id ) THEN 
		UPDATE locprimk2 SET 
				dokid = _dokid,
				aid = _aid,
				naziv = _naziv,
				kolicina = _kolicina,
				jmj = _jmj,
				fcijenajedinicna = _fcijenajedinicna,
				fcijenaukupno = _fcijenaukupno,
				frabatp = _frabatp,
				frabatknjed = _frabatknjed,
				frabatknukp = _frabatknukp,
				fcijenaukupnoumanjeno = _fcijenaukupnoumanjeno,
				ncukupnoitroskovi = _ncukupnoitroskovi,
				marzap = _marzap,
				marzakn = _marzakn,
				ztdokiznososn = _ztdokiznososn,
				ztdokiznospdv = _ztdokiznospdv,
				ztdokkoefi = _ztdokkoefi,
				ncpdvukupno = _ncpdvukupno,
				ncpdvodbitakp = _ncpdvodbitakp,
				ncpdvodbitakkn = _ncpdvodbitakkn,
				pcjedinicna = _pcjedinicna
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO locprimk2( id, dokid, aid, naziv, kolicina, jmj, fcijenajedinicna, fcijenaukupno, frabatp, frabatknjed, frabatknukp, fcijenaukupnoumanjeno, ncukupnoitroskovi, marzap, marzakn, ztdokiznososn, ztdokiznospdv, ztdokkoefi, ncpdvukupno, ncpdvodbitakp, ncpdvodbitakkn, pcjedinicna)
		VALUES (_id,_dokid,_aid,_naziv,_kolicina,_jmj,_fcijenajedinicna,_fcijenaukupno,_frabatp,_frabatknjed,_frabatknukp,_fcijenaukupnoumanjeno,_ncukupnoitroskovi,_marzap,_marzakn,_ztdokiznososn,_ztdokiznospdv,_ztdokkoefi,_ncpdvukupno,_ncpdvodbitakp,_ncpdvodbitakkn,_pcjedinicna);

		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocPrimk2_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocPrimk2_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM locprimk2 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocPrimk3_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocPrimk3_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _dokid int(10) unsigned,
	 IN _opis varchar(250),
	 IN _iznososn double(14,4),
	 IN _iznospdv double(14,4),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM locprimk3 
	WHERE
		id = _id ) THEN 
		UPDATE locprimk3 SET 
				dokid = _dokid,
				opis = _opis,
				iznososn = _iznososn,
				iznospdv = _iznospdv
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO locprimk3( id, dokid, opis, iznososn, iznospdv)
		VALUES (_id,_dokid,_opis,_iznososn, _iznospdv);

		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocPrimk3_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocPrimk3_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM locprimk3 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac1opis_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac1opis_AddUpdate`(
	 IN _rid int(10) unsigned,
	 IN _opis varchar(1024)
)
BEGIN 

	IF EXISTS(SELECT * FROM locrac1opis 
	WHERE
		rid = _rid ) THEN 
		UPDATE locrac1opis SET 
				opis = _opis
		WHERE
			rid = _rid ;
	ELSE
		INSERT INTO locrac1opis( rid, opis)
		VALUES (_rid,_opis);

	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac1opis_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac1opis_Delete`(
	 IN _rid int(10) unsigned
)
BEGIN 

	DELETE FROM locrac1opis 
	WHERE
			rid = _rid;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac1_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac1_AddUpdate`(
	 IN _id int(10) unsigned,
	 OUT _br int(6),
	 IN _vrsta enum('M','V','O','P','A'),
	 IN _idk int(10) unsigned,
	 IN _sum double(14,4),
	 IN _bpdv double(14,4),
	 OUT _datumr datetime,
	 IN _uid int(10) unsigned,
	 IN _obracun enum('N','Y'),
	 IN _stol varchar(10),
	 IN _np int(3),
	 IN _storno enum('0','1'),
	 IN _rabatp double(14,4),
	 IN _rabatk double(14,4),
	 IN _rsum double(14,4),
	 IN _rbpdv double(14,4),
	 IN _storno_datum datetime,
	 IN _oib char(11),
	 IN _zki char(32),
	 IN _sid int(10) unsigned,
	 IN _cid int(10) unsigned,
	 IN _kasaid int(10),
	 IN _oznpp varchar(20),
	 IN _datumv date,
	 IN _oznsljed ENUM('N','P'),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 
	IF EXISTS(SELECT * FROM locrac1 
	WHERE
		id = _id ) THEN 
		UPDATE locrac1 SET 
				vrsta = _vrsta,
				idk = _idk,
				sum = _sum,
				bpdv = _bpdv,
				uid = _uid,
				obracun = _obracun,
				stol = _stol,
				np = _np,
				storno = _storno,
				rabatp = _rabatp,
				rabatk = _rabatk,
				rsum = _rsum,
				rbpdv = _rbpdv,
				storno_datum = _storno_datum,
				oib = _oib,
				zki = _zki,
				sid = _sid,
				cid = _cid,
				kasaid = _kasaid,
				oznpp = _oznpp,
				datumv = _datumv
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO locrac1( id, br, vrsta, idk, sum, bpdv, datumr, uid, obracun, stol, np, storno, rabatp, rabatk, rsum, rbpdv, storno_datum, oib, zki, sid, cid, kasaid, oznpp, datumv)
		SELECT _id,ifnull(max(br)+1,1),_vrsta,_idk,_sum,_bpdv,NOW(),_uid,_obracun,_stol,_np,_storno,_rabatp,_rabatk,_rsum,_rbpdv,_storno_datum,_oib,_zki,_sid,_cid,_kasaid,_oznpp,_datumv 
		FROM locrac1 WHERE vrsta = _vrsta AND kasaid = _kasaid AND oznpp = _oznpp;
		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;

	SELECT br INTO _br FROM locrac1
	WHERE id = _lastInsertID ;
	SELECT datumr INTO _datumr FROM locrac1
	WHERE id = _lastInsertID ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac1_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac1_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 
	DELETE FROM locrac1 
	WHERE
		id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac2opis_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac2opis_AddUpdate`(
	 IN _r2id int(10) unsigned,
	 IN _opis text
)
BEGIN 

	IF EXISTS(SELECT * FROM locrac2opis
	WHERE
		r2id = _r2id ) THEN 
		UPDATE locrac2opis SET 
				opis = _opis
		WHERE
			r2id = _r2id ;
	ELSE
		INSERT INTO locrac2opis( r2id, opis)
		VALUES (_r2id,_opis);

	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac2opis_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac2opis_Delete`(
	 IN _r2id int(10) unsigned
)
BEGIN 
	DELETE FROM locrac2opis 
	WHERE
		r2id = _r2id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac2_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac2_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _rid int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _artsif VARCHAR(30),
	 IN _jmjsifra char(5),
	 IN _kol double(14,4),
	 IN _nc double(14,4),
	 IN _pcbpdv double(14,4),
	 IN _pc double(14,4),
	 IN _ukupno double(14,4),
	 IN _bpdv double(14,4),
	 IN _rabatP double(14,4),
	 IN _rabatK double(14,4),
	 IN _ukpcisti double(14,4),
	 IN _naziv varchar(200),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM locrac2 
	WHERE
		id = _id ) THEN 
		UPDATE locrac2 SET 
				rid = _rid,
				aid = _aid,
				artsif = _artsif,
				kol = _kol,
				nc = _nc,
				pcbpdv = _pcbpdv,
				pc = _pc,
				ukupno = _ukupno,
				bpdv = _bpdv,
				rabatP = _rabatP,
				rabatK = _rabatK,
				ukpcisti = _ukpcisti,
				naziv = _naziv
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO locrac2( id, rid, aid, artsif, jmjsifra, kol, nc, pcbpdv, pc, ukupno, bpdv, rabatP, rabatK, ukpcisti, naziv)
		VALUES (_id, _rid,_aid,_artsif,_jmjsifra,_kol,_nc,_pcbpdv,_pc,_ukupno,_bpdv,_rabatP,_rabatK,_ukpcisti,_naziv);
		
		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_LocRac2_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_LocRac2_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM locrac2 
	WHERE
		id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Lookup_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Lookup_AddUpdate`(
	 IN _pojam char(2),
	 IN _sifra char(3),
	 IN _opis varchar(20),
	 IN _value1 double(14,7),
	 IN _value2 double(14,7),
	 IN _value3 double(14,7),
	 IN _value4 double(14,7),
	 IN _id int(10) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM lookup 
	WHERE
		id = _id ) THEN 
		UPDATE lookup SET 
				pojam = _pojam,
				sifra = _sifra,
				opis = _opis,
				value1 = _value1,
				value2 = _value2,
				value3 = _value3,
				value4 = _value4
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO lookup( pojam, sifra, opis, value1, value2, value3, value4, id)
		VALUES (_pojam,_sifra,_opis,_value1,_value2,_value3,_value4,_id);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Lookup_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Lookup_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM lookup 
	WHERE
		id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Matsklad_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Matsklad_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _sifra char(10),
	 IN _naziv char(50),
	 IN _opis char(50),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM matsklad 
	WHERE
		id = _id ) THEN 
		UPDATE matsklad SET 
				sifra = _sifra,
				naziv = _naziv,
				opis = _opis
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO matsklad( id, sifra, naziv, opis)
		VALUES (_id,_sifra,_naziv,_opis);

		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Matsklad_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Matsklad_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM matsklad 
	WHERE
			id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Mat_JediniceMjere_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Mat_JediniceMjere_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _sifra char(5),
	 IN _opis varchar(20),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM mat_jedinicemjere 
	WHERE
		id = _id ) THEN 
		UPDATE mat_jedinicemjere SET 
				sifra = _sifra,
				opis = _opis
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO mat_jedinicemjere( id, sifra, opis)
		VALUES (_id,_sifra,_opis);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Mat_JediniceMjere_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Mat_JediniceMjere_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM mat_jedinicemjere 
	WHERE
		id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_NacinPlacanja_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_NacinPlacanja_AddUpdate`(
	 IN _id int(3),
	 IN _naziv varchar(200),
	 IN _alias varchar(100),
	 IN _dozvola enum('N','Y'),
	 IN _fiskal_znak char(1),
	OUT _lastInsertID  int(3)
)
BEGIN 

	IF EXISTS(SELECT * FROM kartice 
	WHERE
		id = _id ) THEN 
		UPDATE kartice SET 
				naziv = _naziv,
				alias = _alias,
				dozvola = _dozvola,
				fiskal_znak = _fiskal_znak
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO kartice( id, naziv, alias, dozvola, fiskal_znak)
		VALUES (_id,_naziv,_alias,_dozvola,_fiskal_znak);

		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_NacinPlacanja_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_NacinPlacanja_Delete`(
	 IN _id int(3)
)
BEGIN 

	DELETE FROM kartice 
	WHERE
		id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Normativ_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Normativ_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _nid int(10) unsigned,
	 IN _kolicina double(14,4),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM normativ 
	WHERE
		id = _id ) THEN 
		UPDATE normativ SET 
				aid = _aid,
				nid = _nid,
				kolicina = _kolicina
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO normativ( id, aid, nid, kolicina)
		VALUES (_id,_aid,_nid,_kolicina);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Normativ_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Normativ_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM normativ 
	WHERE
			id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Porez_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Porez_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _sifra char(5),
	 IN _opis varchar(50),
	 IN _postotak double(14,7),
	 IN _faktorPunaCijenaPorez double(14,7),
	 IN _faktorPunaCijenaOsnovica double(14,7),
	 IN _poreztip char(5),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM porez 
	WHERE
		id = _id ) THEN 
		UPDATE porez SET 
				sifra = _sifra,
				opis = _opis,
				postotak = _postotak,
				faktorPunaCijenaPorez = _faktorPunaCijenaPorez,
				faktorPunaCijenaOsnovica = _faktorPunaCijenaOsnovica,
				poreztip = _poreztip
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO porez( id, sifra, opis, postotak, faktorPunaCijenaPorez, faktorPunaCijenaOsnovica, poreztip)
		VALUES (_id,_sifra,_opis,_postotak,_faktorPunaCijenaPorez,_faktorPunaCijenaOsnovica, _poreztip);

		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Porez_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Porez_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM porez 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Primk1_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Primk1_AddUpdate`(
	 IN _id int(10) unsigned,
	OUT _dokbr int(10) unsigned,
	 IN _dobavljacID int(10) unsigned,
	 IN _datumdok date,
	 IN _datumv date,
	 IN _skladID int(10) unsigned,
	 IN _tipfID int(10) unsigned,
	 IN _vezadokument varchar(100),
	 IN _fiznosstart double(14,4),
	 IN _fiznosrabatkn double(14,4),
	 IN _fiznosukupno double(14,4),
	 IN _fiznospdv double(14,4),
	 IN _ztiznososn double(14,4),
	 IN _ztiznospdv double(14,4),
	 IN _ztkoefi double(14,4),
	 IN _iznosukupno double(14,4),
	 IN _valsifra char(3),
	 IN _valdatum date,
	 IN _valtecaj double(14,4),
	 IN _valiznos double(14,4),
	 IN _storno enum('0','1'),
	 IN _storno_datum date,
	 IN _kasaid int(10),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM primk1 
	WHERE
		id = _id ) THEN 
		UPDATE primk1 SET 
				dobavljacID = _dobavljacID,
				datumdok = _datumdok,
				datumv = _datumv,
				skladID = _skladID,
				tipfID = _tipfID,
				vezadokument = _vezadokument,
				fiznosstart = _fiznosstart,
				fiznosrabatkn = _fiznosrabatkn,
				fiznosukupno = _fiznosukupno,
				fiznospdv = _fiznospdv,
				ztiznososn = _ztiznososn,
				ztiznospdv = _ztiznospdv,
				ztkoefi = _ztkoefi,
				iznosukupno = _iznosukupno,
				valsifra = _valsifra,
				valdatum = _valdatum,
				valtecaj = _valtecaj,
				valiznos = _valiznos,
				storno = _storno,
				storno_datum = _storno_datum,
				kasaid = _kasaid
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO primk1( id, dokbr, dobavljacID, datumdok, datumv, skladID, tipfID, vezadokument, fiznosstart, fiznosrabatkn, fiznosukupno, fiznospdv, ztiznososn, ztiznospdv, ztkoefi, iznosukupno, valsifra, valdatum, valtecaj, valiznos, storno, storno_datum, kasaid)
		SELECT _id,ifnull(max(dokbr)+1,1),_dobavljacID,_datumdok,_datumv,_skladID,_tipfID,_vezadokument,_fiznosstart,_fiznosrabatkn,_fiznosukupno,_fiznospdv,_ztiznososn,_ztiznospdv,_ztkoefi,_iznosukupno,_valsifra,_valdatum,_valtecaj,_valiznos,_storno,_storno_datum,_kasaid
		FROM primk1;

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;

	SELECT dokbr INTO _dokbr FROM primk1
	WHERE id = _lastInsertID ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Primk1_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Primk1_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM primk1 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Primk2_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Primk2_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _dokid int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _naziv varchar(250),
	 IN _kolicina double(14,4),
	 IN _jmj char(5),
	 IN _fcijenajedinicna double(14,4),
	 IN _fcijenaukupno double(14,4),
	 IN _frabatp double(14,4),
	 IN _frabatknjed double(14,4),
	 IN _frabatknukp double(14,4),
	 IN _fcijenaukupnoumanjeno double(14,4),
	 IN _ncukupnoitroskovi double(14,4),
	 IN _marzap double(14,4),
	 IN _marzakn double(14,4),
	 IN _ztdokiznososn double(14,4),
	 IN _ztdokiznospdv double(14,4),
	 IN _ztdokkoefi double(14,4),
	 IN _ncpdvukupno double(14,4),
	 IN _ncpdvodbitakp double(14,4),
	 IN _ncpdvodbitakkn double(14,4),
	 IN _pcjedinicna double(14,4),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM primk2 
	WHERE
		id = _id ) THEN 
		UPDATE primk2 SET 
				dokid = _dokid,
				aid = _aid,
				naziv = _naziv,
				kolicina = _kolicina,
				jmj = _jmj,
				fcijenajedinicna = _fcijenajedinicna,
				fcijenaukupno = _fcijenaukupno,
				frabatp = _frabatp,
				frabatknjed = _frabatknjed,
				frabatknukp = _frabatknukp,
				fcijenaukupnoumanjeno = _fcijenaukupnoumanjeno,
				ncukupnoitroskovi = _ncukupnoitroskovi,
				marzap = _marzap,
				marzakn = _marzakn,
				ztdokiznososn = _ztdokiznososn,
				ztdokiznospdv = _ztdokiznospdv,
				ztdokkoefi = _ztdokkoefi,
				ncpdvukupno = _ncpdvukupno,
				ncpdvodbitakp = _ncpdvodbitakp,
				ncpdvodbitakkn = _ncpdvodbitakkn,
				pcjedinicna = _pcjedinicna
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO primk2( id, dokid, aid, naziv, kolicina, jmj, fcijenajedinicna, fcijenaukupno, frabatp, frabatknjed, frabatknukp, fcijenaukupnoumanjeno, ncukupnoitroskovi, marzap, marzakn, ztdokiznososn, ztdokiznospdv, ztdokkoefi, ncpdvukupno, ncpdvodbitakp, ncpdvodbitakkn, pcjedinicna)
		VALUES (_id,_dokid,_aid,_naziv,_kolicina,_jmj,_fcijenajedinicna,_fcijenaukupno,_frabatp,_frabatknjed,_frabatknukp,_fcijenaukupnoumanjeno,_ncukupnoitroskovi,_marzap,_marzakn,_ztdokiznososn,_ztdokiznospdv,_ztdokkoefi,_ncpdvukupno,_ncpdvodbitakp,_ncpdvodbitakkn,_pcjedinicna);

		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Primk2_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Primk2_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM primk2 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Primk3_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Primk3_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _dokid int(10) unsigned,
	 IN _opis varchar(250),
	 IN _iznososn double(14,4),
	 IN _iznospdv double(14,4),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM primk3 
	WHERE
		id = _id ) THEN 
		UPDATE primk3 SET 
				dokid = _dokid,
				opis = _opis,
				iznososn = _iznososn,
				iznospdv = _iznospdv
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO primk3( id, dokid, opis, iznososn, iznospdv)
		VALUES (_id,_dokid,_opis,_iznososn, _iznospdv);

		SET _lastInsertID = LAST_INSERT_ID();
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Primk3_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Primk3_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM primk3 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_rac3_insert_normativ` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_rac3_insert_normativ`(
	 IN _racunID int(6),
	 IN _aid int(10) unsigned,
	 IN _nid int(10) unsigned,
	 IN _kolicina double(14,4)
)
BEGIN
	DECLARE newKolicina double(14,4);
	DECLARE newartnorm_id int(10) unsigned;
	DECLARE done INT DEFAULT FALSE;
	DECLARE counter INT DEFAULT 0;
	
	DECLARE cursorNorm CURSOR FOR 
		SELECT nid, kolicina FROM normativ 
		WHERE aid = _nid;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
	
	OPEN cursorNorm;

read_loop: LOOP
	FETCH cursorNorm INTO newartnorm_id, newKolicina;
	IF done THEN
		LEAVE read_loop;
    END IF;
	SET counter = counter + 1;
	SET newKolicina = newKolicina * _kolicina;
	CALL proc_rac3_insert_normativ(_racunID, _aid, newartnorm_id, newKolicina);

	END LOOP;
	CLOSE cursorNorm;
	IF counter = 0 THEN
		INSERT INTO rac3( rid, aid, nid, kol)
		VALUES (_racunID,_aid,_nid,_kolicina);
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Recalculate_Skladista` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Recalculate_Skladista`()
BEGIN
        DECLARE r2Kolicina double(14,4);
        DECLARE r2AID int(10) unsigned;
        DECLARE r2RID int(6);
        DECLARE counter INT DEFAULT 0;

        DECLARE p2aid int(10) unsigned;
        DECLARE p2sid int(10);
        DECLARE p2kolicina double(14,4);
        DECLARE p2artsif varchar(30);

		DECLARE mSIDsa int(10);
        DECLARE mSIDna int(10);

		DECLARE RNskinuto tinyint(4);
		DECLARE artUsluga enum('N','Y');

        DECLARE done INT DEFAULT FALSE;
        

        DECLARE cursorRac2 CURSOR FOR 
                SELECT rid, aid, kol FROM rac2; 

        DECLARE cursorVrac2 CURSOR FOR 
                SELECT rid, aid, kol FROM vrac1 v1, vrac2 v2
				WHERE v1.vrsta IN ('M', 'V', 'O') 
				AND v1.id = v2.rid; 

        DECLARE cursorPrimk2 CURSOR FOR 
                SELECT p1.skladID, p2.aid, a.artsif, p2.kolicina
                FROM primk1 p1, primk2 p2, art a
                WHERE p1.id = p2.dokID
				AND p2.aid = a.id; 

        DECLARE cursorMSK2 CURSOR FOR 
			SELECT SIDsa, SIDna, aid, artsif, kolicina
			FROM msk2 JOIN msk1 ON msk2.mskid = msk1.id;

        DECLARE cursorRN CURSOR FOR 
			SELECT rn3.aid, rn3.artsif, rn3.kol, rn3.sid, rn3.skinuto, a.usluga
			FROM rnalog3 rn3 JOIN art a ON rn3.aid = a.id where rn3.skinuto=1;

        DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

        truncate rac3;
        truncate vrac3;
        truncate sklad;


        OPEN cursorRac2;
read_loop0: LOOP
        FETCH cursorRac2 INTO r2RID, r2AID, r2Kolicina;
        IF done THEN
			LEAVE read_loop0;
		END IF;
        CALL proc_rac3_insert_normativ(r2RID, r2AID, r2AID, r2Kolicina);

        END LOOP;
        CLOSE cursorRac2;
        SET done = false;       


        OPEN cursorVrac2;
read_loop1: LOOP
        FETCH cursorVrac2 INTO r2RID, r2AID, r2Kolicina;
        IF done THEN
            LEAVE read_loop1;
		END IF;
        CALL proc_vrac3_insert_normativ(r2RID, r2AID, r2AID, r2Kolicina);

        END LOOP;
        CLOSE cursorVrac2;
        SET done = false;       


        OPEN cursorPrimk2;
read_loop2: LOOP
        FETCH cursorPrimk2 INTO p2sid, p2aid, p2artsif, p2kolicina;
        IF done THEN
			LEAVE read_loop2;
		END IF;
        CALL proc_Update_Skladista(p2aid, p2sid, p2kolicina, p2artsif);

        END LOOP;
        CLOSE cursorPrimk2;
        SET done = false;  


        OPEN cursorMSK2;
read_loop3: LOOP
        FETCH cursorMSK2 INTO mSIDsa, mSIDna, p2aid, p2artsif, p2kolicina;
        IF done THEN
			LEAVE read_loop3;
		END IF;
		CALL proc_Update_Skladista(p2aid,mSIDsa,(p2kolicina*-1),p2artsif);
        CALL proc_Update_Skladista(p2aid,mSIDna,p2kolicina,p2artsif);

        END LOOP;
        CLOSE cursorMSK2;
        SET done = false;  


        OPEN cursorRN;
read_loop4: LOOP
        FETCH cursorRN INTO p2aid, p2artsif, p2kolicina, p2sid, RNskinuto, artUsluga;
        IF done THEN
			LEAVE read_loop4;
		END IF;
		IF (RNskinuto = 1 and artUsluga='N') THEN
			CALL proc_Update_Skladista(p2aid, p2sid, (p2kolicina*-1), p2artsif);
		END IF;

        END LOOP;
        CLOSE cursorRN;
        SET done = false;  
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Recalculate_Skladista_Vrijeme` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Recalculate_Skladista_Vrijeme`(
	IN _DatumOd datetime,
	IN _DatumDo datetime
)
BEGIN
        DECLARE r2Kolicina double(14,4);
        DECLARE r2AID int(10) unsigned;
        DECLARE r2RID int(10) unsigned;
        DECLARE counter INT DEFAULT 0;

        DECLARE p2aid int(10);
        DECLARE p2sid int(10);
        DECLARE p2kolicina double(14,4);
        DECLARE p2artsif varchar(30);
        
	DECLARE RNskinuto tinyint(4);
	DECLARE artUsluga enum('N','Y');

        DECLARE done INT DEFAULT FALSE;
        
        DECLARE cursorRac2 CURSOR FOR 
                SELECT r2.rid, r2.aid, r2.kol FROM rac2 r2 left join rac1 r1 on
				r2.rid=r1.id where r1.datumr between _DatumOd and _DatumDo;

        DECLARE cursorPrimk2 CURSOR FOR 
                SELECT p1.skladid, p2.aid, p2.artsif, p2.kolicina
                FROM primk1 p1, primk2 p2
                WHERE p1.datumdok between date(_DatumOd) and date(_DatumDo) and p1.id = p2.dokid; 


        DECLARE cursorRN CURSOR FOR 
			SELECT rn3.aid, rn3.artsif, rn3.kol, rn3.sid, rn3.skinuto, a.usluga
			FROM rnalog3 rn3 JOIN art a ON rn3.aid = a.id left join rnalog1 rn1
			on rn3.rid=rn1.id 
			where date(rn1.datumz) between date(_DatumOd) and date(_DatumDo);

        DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
        
        truncate sklad;

        OPEN cursorRac2;
read_loop: LOOP
        FETCH cursorRac2 INTO r2RID, r2AID, r2Kolicina;
        IF done THEN
                LEAVE read_loop;
    END IF;
		delete from rac3 where rid=r2RID;
        CALL proc_rac3_insert_normativ(r2RID, r2AID, r2AID, r2Kolicina);

        END LOOP;
        CLOSE cursorRac2;
        SET done = false;       

        OPEN cursorPrimk2;
read_loop2: LOOP
        FETCH cursorPrimk2 INTO p2sid, p2aid, p2artsif, p2kolicina;
        IF done THEN
                LEAVE read_loop2;
    END IF;
        CALL proc_Update_Skladista(p2aid, p2sid, p2kolicina, p2artsif);

        END LOOP;
        CLOSE cursorPrimk2;
		SET done = false;       
		
        OPEN cursorRN;
read_loop3: LOOP
        FETCH cursorRN INTO p2aid, p2artsif, p2kolicina, p2sid, RNskinuto, artUsluga;
        IF done THEN
			LEAVE read_loop3;
		END IF;
		IF (RNskinuto = 1 and artUsluga='N') THEN
			CALL proc_Update_Skladista(p2aid, p2sid, (p2kolicina*-1), p2artsif);
		END IF;

        END LOOP;
        CLOSE cursorRN;
        SET done = false;  

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_RekreirajBrojeveRacuna` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_RekreirajBrojeveRacuna`()
BEGIN
drop table if exists br1;
CREATE temporary TABLE `br1` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `br` int(6) NOT NULL COMMENT 'broj racuna',
  `godina` int(4) NOT NULL COMMENT 'godina izrade racuna',
  `vrsta` enum('R','O','P','RN') DEFAULT NULL,
  `kasaid` int(10) NOT NULL COMMENT 'ID naplatnog uredaja',
  `oznpp` varchar(20) NOT NULL COMMENT 'Oznaka poslovnog prostora',
  `oznsljed` enum('N','P') NOT NULL COMMENT 'N - brojevi na nivou naplatnog uredaja, P - brojevi na nivou poslovnog prostora',
  `datum` datetime NOT NULL,
  `vrstareal` enum('M','V','O','P','RN') DEFAULT NULL,
  `iddoc` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_brojracuna_brvrsta` (`br`,`vrsta`),
  KEY `idx_brojracuna_godina` (`godina`),
  KEY `idx_brojracuna_kasaidoznpp` (`kasaid`,`oznpp`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

insert into br1 select 0 id,br,year(datumr) datum,'R' vrsta,kasaid,oznpp,'N',datumr,'M' vrstareal,id iddoc from rac1;
insert into br1 select 0 id,br,year(datumr) datum,if(vrsta='V','R',vrsta) vrstaR,kasaid,oznpp,'N',datumr,vrsta vrstareal,id iddoc from vrac1;
insert into br1 select 0 id,dok,year(datumz) datum,'RN' vrsta,1,'PJ1','N',datumz,'RN' vrstareal,id iddoc from rnalog1 where zatvoren=1;
truncate brojracuna;
insert into brojracuna select 0,br,godina,vrsta,kasaid,oznpp,oznsljed,datum,vrstareal,iddoc from br1 order by datum;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_RekreirajSerbr` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_RekreirajSerbr`(
	IN _SID int(10)
)
BEGIN
truncate serbr;

insert into serbr (vrsta,ulazizlaz,imanema,datum,dokid,sid,aid,kol,nc) select 'primk','U','Y',
p1.datumdok,p1.id,p1.skladid,p2.aid,p2.kolicina,p2.ncukupnoitroskovi/p2.kolicina 
from primk2 p2 left join primk1 p1 on p2.dokid=p1.id where p1.skladid=_SID;

insert into serbr (vrsta,ulazizlaz,imanema,datum,dokid,sid,aid,kol,nc) select 'rac3','I','N',r1.datumr,
r1.id,r1.sid,r3.nid,r3.kol*-1,0 from rac3 r3 left join rac1 r1 on r3.rid=r1.id where r1.sid=_SID;

insert into serbr (vrsta,ulazizlaz,imanema,datum,dokid,sid,aid,kol,nc) select 'rac3','I','N',r1.datumr,
r1.id,r1.sid,r3.nid,r3.kol*-1,0 from vrac3 r3 left join vrac1 r1 on r3.rid=r1.id where r1.vrsta in ('V','O') and r1.sid=_SID;

insert into serbr (vrsta,ulazizlaz,imanema,datum,dokid,sid,aid,kol,nc) select 'rnalog3','I','N',r1.datumz,
r1.id,r3.sid,r3.aid,r3.kol*-1,0 from rnalog3 r3 left join rnalog1 r1 on r3.rid=r1.id where r3.skinuto='1' and r3.sid=_SID;

insert into serbr (vrsta,ulazizlaz,imanema,datum,dokid,sid,aid,kol,nc) select 'msk3','U','Y',m1.datum,
m1.id,m1.sidna,m2.aid,m2.kolicina,0 from msk2 m2 left join msk1 m1 on m2.mskid=m1.id where m1.sidna=_SID;
insert into serbr (vrsta,ulazizlaz,imanema,datum,dokid,sid,aid,kol,nc) select 'msk3','I','N',m1.datum,
m1.id,m1.sidsa,m2.aid,m2.kolicina*-1,0 from msk2 m2 left join msk1 m1 on m2.mskid=m1.id where m1.sidsa=_SID;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_RekreirajSkladPoSerbr` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_RekreirajSkladPoSerbr`()
BEGIN
truncate sklad;
insert into sklad (artsif,aid,sid,kol,pcijena) select a.artsif,a.id,s.id,0,0 from art a inner join matsklad s;
update sklad s,(select aid,sid,sum(kol) k from serbr group by aid,sid) ss set s.kol=ss.k where s.aid=ss.aid and s.sid=ss.sid;
delete from sklad where kol=0;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_RobnaKartica` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_RobnaKartica`(
    IN _SID int(10),
    IN _AID int(10)
)
BEGIN
select p1.datumdok datumdok,p1.skladid sid,"Primk" vrsta,p1.dokbr dokumentbr,p2.aid,p2.naziv,p2.kolicina ulaz,0 izlaz,p2.jmj,p1.vezadokument,ms.naziv naziv_sklad,a.artsif from primk2 p2 left join primk1 p1 on p2.dokid=p1.id left join matsklad ms on p1.skladid=ms.id left join art a on p2.aid=a.id where p1.skladid=_SID and p2.aid=_AID 
union all 
select v1.datumr datumdok,v1.sid sid,"Racun" vrsta,v1.br dokumentbr,v2.nid,a.naziv,0 ulaz,v2.kol izlaz,mjm.sifra,v1.storno_datum,ms.naziv naziv_sklad,a.artsif from rac3 v2 left join rac1 v1 on v2.rid=v1.id left join matsklad ms on v1.sid=ms.id left join art a on v2.nid=a.id left join mat_jedinicemjere mjm on a.jmjid=mjm.id where v1.sid=_SID and v2.nid=_AID
union all 
select v1.datumr datumdok,v1.sid sid,(case when v1.vrsta='V' then 'VRacun' when v1.vrsta='O' then "Otpremnica" end) vrsta,v1.br dokumentbr,v2.nid,a.naziv,0 ulaz,v2.kol izlaz,mjm.sifra,v1.storno_datum,ms.naziv naziv_sklad,a.artsif from vrac3 v2 left join vrac1 v1 on v2.rid=v1.id left join matsklad ms on v1.sid=ms.id left join art a on v2.nid=a.id join mat_jedinicemjere mjm on a.jmjid=mjm.id where v1.vrsta in('M','V','O') and v1.sid=_SID and v2.nid=_AID
union all 
select r1.datumz datumdok,r3.sid,'Rnalog' vrsta,r1.dok dokumentbr,r3.aid,r3.naziv,0 ulaz,r3.kol izlaz,'','',ms.naziv naziv_sklad,'' from rnalog3 r3 left join rnalog1 r1 on r3.rid=r1.id left join matsklad ms on r3.sid=ms.id where r3.sid=_SID and r3.aid=_AID
union all 
select m1.datum datumdok,m1.sidsa sid,"MSKizlaz" vrsta,m1.id dokumentbr,m2.aid,m2.naziv,0 ulaz,m2.kolicina izlaz,'',concat('Na sklad-',(select naziv from matsklad where id=_SID)),ms.naziv naziv_sklad,m2.artsif from msk2 m2 left join msk1 m1 on m2.mskid=m1.id left join matsklad ms on m1.sidsa=ms.id where m1.sidsa=_SID and m2.aid=_AID
union all 
select m1.datum datumdok,m1.sidna sid,"MSKulaz" vrsta,m1.id dokumentbr,m2.aid,m2.naziv,m2.kolicina ulaz,0 izlaz,'',concat('Sa sklad-',(select naziv from matsklad where id=_SID)),ms.naziv naziv_sklad,m2.artsif from msk2 m2 left join msk1 m1 on m2.mskid=m1.id left join matsklad ms on m1.sidna=ms.id where m1.sidna=_SID and m2.aid=_AID order by datumdok;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_UlazneCijene_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_UlazneCijene_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _cijenaulaz double(14,4) unsigned,
	 IN _cijenaulazBPDV double(14,4) unsigned,
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM ulaznecijene 
	WHERE
		id = _id ) THEN 
		UPDATE ulaznecijene SET 
				aid = _aid,
				cijenaulaz = _cijenaulaz,
				cijenaulazBPDV = _cijenaulazBPDV
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO ulaznecijene( id, aid, cijenaulaz, cijenaulazBPDV)
		VALUES (_id,_aid,_cijenaulaz,_cijenaulazBPDV);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_UlazneCijene_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_UlazneCijene_Delete`(
     IN _id int(10) unsigned
)
BEGIN

    DELETE FROM ulaznecijene
    WHERE
            id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Update_Skladista` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Update_Skladista`(
	IN _aid int(10),
	IN _sid int(10),
	IN _kol double(14,4),
	IN _artsif varchar(30)
)
BEGIN
	DECLARE _Ima int;
	set _Ima=(select count(s.aid) from sklad s where s.sid=_sid and s.aid=_aid);
	IF _Ima > 0 THEN
		update sklad set kol=kol+_kol where sid=_sid and aid=_aid;
	ELSE
		insert into sklad (artsif,aid,sid,kol) values(_artsif,_aid,_sid,_kol);  
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac1opis_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac1opis_AddUpdate`(
	 IN _rid int(10) unsigned,
	 IN _opis varchar(1024)
)
BEGIN 

	IF EXISTS(SELECT * FROM vrac1opis
	WHERE
		rid = _rid ) THEN 
		UPDATE vrac1opis SET 
				opis = _opis
		WHERE
			rid = _rid ;
	ELSE
		INSERT INTO vrac1opis( rid, opis)
		VALUES (_rid,_opis);

	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac1opis_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac1opis_Delete`(
	 IN _rid int(10) unsigned
)
BEGIN 

	DELETE FROM vrac1opis 
	WHERE
			rid = _rid;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac1Porezi_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac1Porezi_AddUpdate`(
	 IN _rid int(10) unsigned,
	 IN _pid int(10) unsigned,
	 IN _porez_sifra char(5),
	 IN _iznos_poreza double(14,4),
	 IN _porez_postotak double(14,7)
)
BEGIN 

	IF EXISTS(SELECT * FROM vrac1porezi 
	WHERE
		rid = _rid AND
		pid = _pid ) THEN 
		UPDATE vrac1porezi SET 
				porez_sifra = _porez_sifra,
				iznos_poreza = _iznos_poreza,
				porez_postotak = _porez_postotak
		WHERE
			rid = _rid AND
			pid = _pid ;
	ELSE
		INSERT INTO vrac1porezi( rid, pid, porez_sifra, iznos_poreza, porez_postotak)
		VALUES (_rid,_pid,_porez_sifra,_iznos_poreza,_porez_postotak);

	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac1Porezi_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac1Porezi_Delete`(
	 IN _rid int(10) unsigned,
	 IN _pid int(10) unsigned
)
BEGIN 

	DELETE FROM vrac1porezi 
	WHERE
			rid = _rid AND
		pid = _pid ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Vrac1Vrac1_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Vrac1Vrac1_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _vrac1racID int(10) unsigned,
	 IN _vrac1vezID int(10) unsigned,
	 IN _storno enum('1','0'),
	 IN _tip enum('MO','MP','VO','VP','VA'),
	 IN _iznos double(14,4),
	 IN _pdv double(14,4),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 
	DECLARE _used enum('1','0');

	IF EXISTS(SELECT * FROM vrac1vrac1conn 
	WHERE
		id = _id ) THEN 
		UPDATE vrac1vrac1conn SET 
				vrac1racID = _vrac1racID,
				vrac1vezID = _vrac1vezID,
				storno = _storno,
				tip = _tip,
				iznos = _iznos,
				pdv = _pdv
		WHERE
			id = _id ;
        SET _lastInsertID = _id;
	ELSE
		INSERT INTO vrac1vrac1conn( id, vrac1racID, vrac1vezID, storno, tip, iznos, pdv)
		VALUES (_id,_vrac1racID,_vrac1vezID,_storno,_tip,_iznos,_pdv);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
	
	IF (_storno = '1') THEN
		SET _used = '0';
	ELSE
		SET _used = '1';
	END IF;

	UPDATE vrac1
	SET used = _used
	WHERE id = _vrac1vezID;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_Vrac1Vrac1_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_Vrac1Vrac1_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM vrac1vrac1conn 
	WHERE
			id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac1_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac1_AddUpdate`(
	 IN _id int(10) unsigned,
	 OUT _br int(6),
	 IN _vrsta enum('M','V','O','P','A'),
	 IN _idk int(10) unsigned,
	 IN _sum double(14,4),
	 IN _bpdv double(14,4),
	 OUT _datumr datetime,
	 IN _uid int(10) unsigned,
	 IN _obracun enum('N','Y'),
	 IN _stol varchar(10),
	 IN _np int(3),
	 IN _storno enum('0','1'),
	 IN _rabatp double(14,4),
	 IN _rabatk double(14,4),
	 IN _rsum double(14,4),
	 IN _rbpdv double(14,4),
	 IN _storno_datum datetime,
	 IN _oib char(11),
	 IN _zki char(32),
	 IN _sid int(10) unsigned,
	 IN _cid int(10) unsigned,
	 IN _kasaid int(10),
	 IN _oznpp varchar(20),
	 IN _datumv date,
	 IN _oznsljed ENUM('N','P'),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 
	IF EXISTS(SELECT * FROM vrac1 
	WHERE
		id = _id ) THEN 
		UPDATE vrac1 SET 
				vrsta = _vrsta,
				idk = _idk,
				sum = _sum,
				bpdv = _bpdv,
				uid = _uid,
				obracun = _obracun,
				stol = _stol,
				np = _np,
				storno = _storno,
				rabatp = _rabatp,
				rabatk = _rabatk,
				rsum = _rsum,
				rbpdv = _rbpdv,
				storno_datum = _storno_datum,
				oib = _oib,
				zki = _zki,
				sid = _sid,
				cid = _cid,
				kasaid = _kasaid,
				oznpp = _oznpp,
				datumv = _datumv
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO vrac1( id, br, vrsta, idk, sum, bpdv, datumr, uid, obracun, stol, np, storno, rabatp, rabatk, rsum, rbpdv, storno_datum, oib, zki, sid, cid, kasaid, oznpp, datumv)
		VALUES (_id,0,_vrsta,_idk,_sum,_bpdv,NOW(),_uid,_obracun,_stol,_np,_storno,_rabatp,_rabatk,_rsum,_rbpdv,_storno_datum,_oib,_zki,_sid,_cid,_kasaid,_oznpp,_datumv);

		SET _lastInsertID = LAST_INSERT_ID(); 

		CALL proc_DohvatBrojaDokumenta(_vrsta,_kasaid,_oznpp,_oznsljed,_lastInsertID,@brojRac);
		
		UPDATE vrac1 SET br = @brojRac WHERE id = _lastInsertID;
	END IF;

	SELECT br INTO _br FROM vrac1
	WHERE id = _lastInsertID ;
	SELECT datumr INTO _datumr FROM vrac1
	WHERE id = _lastInsertID ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac1_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac1_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 
	DELETE FROM vrac1 
	WHERE
		id = _id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac2opis_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac2opis_AddUpdate`(
	 IN _r2id int(10) unsigned,
	 IN _opis text
)
BEGIN 
	IF EXISTS(SELECT * FROM vrac2opis
	WHERE
		r2id = _r2id ) THEN 
		UPDATE vrac2opis SET 
				opis = _opis
		WHERE
			r2id = _r2id ;
	ELSE
		INSERT INTO vrac2opis( r2id, opis)
		VALUES (_r2id,_opis);
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac2opis_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac2opis_Delete`(
	 IN _r2id int(10) unsigned
)
BEGIN 
	DELETE FROM vrac2opis 
	WHERE
		r2id = _r2id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac2Porezi_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac2Porezi_AddUpdate`(
	 IN _rid int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _pid int(10) unsigned,
	 IN _koeficijent_poreza double(14,6),
	 IN _iznos_poreza double(14,6),
	 IN _porez_opis char(5),
	 IN _porez_postotak double(14,7),
	 IN _r2ID int(10) unsigned,
	 IN _osnovica double(14,6)
)
BEGIN 

	IF EXISTS(SELECT * FROM vrac2porezi 
	WHERE
		rid = _rid AND
		aid = _aid AND
		pid = _pid AND
		r2id = _r2ID ) THEN 
		UPDATE vrac2porezi SET 
			koeficijent_poreza = _koeficijent_poreza,
			iznos_poreza = _iznos_poreza,
			porez_opis = _porez_opis,
			porez_postotak = _porez_postotak,
			r2ID = _r2ID,
			osnovica = _osnovica
		WHERE
			rid = _rid AND
			aid = _aid AND
			pid = _pid AND
			r2id = _r2ID;
	ELSE
		INSERT INTO vrac2porezi( rid, aid, pid, koeficijent_poreza, iznos_poreza, porez_opis, porez_postotak, r2ID, osnovica)
		VALUES (_rid,_aid,_pid,_koeficijent_poreza,_iznos_poreza,_porez_opis, _porez_postotak, _r2ID, _osnovica);
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac2Porezi_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac2Porezi_Delete`(
	 IN _rid int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _pid int(10) unsigned
)
BEGIN 

	DELETE FROM vrac2porezi 
	WHERE
		rid = _rid AND
		aid = _aid AND
		pid = _pid ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac2_AddUpdate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac2_AddUpdate`(
	 IN _id int(10) unsigned,
	 IN _rid int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _artsif VARCHAR(30),
	 IN _jmjsifra char(5),
	 IN _kol double(14,4),
	 IN _nc double(14,4),
	 IN _pcbpdv double(14,4),
	 IN _pc double(14,4),
	 IN _ukupno double(14,4),
	 IN _bpdv double(14,4),
	 IN _rabatP double(14,4),
	 IN _rabatK double(14,4),
	 IN _ukpcisti double(14,4),
	 IN _naziv varchar(200),
	OUT _lastInsertID  int(10) unsigned
)
BEGIN 

	IF EXISTS(SELECT * FROM vrac2 
	WHERE
		id = _id ) THEN 
		UPDATE vrac2 SET 		
				rid = _rid,
				aid = _aid,
				artsif = _artsif,
				kol = _kol,
				nc = _nc,
				pcbpdv = _pcbpdv,
				pc = _pc,
				ukupno = _ukupno,
				bpdv = _bpdv,
				rabatP = _rabatP,
				rabatK = _rabatK,
				ukpcisti = _ukpcisti,
				naziv = _naziv
		WHERE
			id = _id ;
		SET _lastInsertID = _id;
	ELSE
		INSERT INTO vrac2( id, rid, aid, artsif, jmjsifra, kol, nc, pcbpdv, pc, ukupno, bpdv, rabatP, rabatK, ukpcisti, naziv)
		VALUES (_id, _rid,_aid,_artsif,_jmjsifra,_kol,_nc,_pcbpdv,_pc,_ukupno,_bpdv,_rabatP,_rabatK,_ukpcisti,_naziv);

		SET _lastInsertID = LAST_INSERT_ID(); 
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_VRac2_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_VRac2_Delete`(
	 IN _id int(10) unsigned
)
BEGIN 

	DELETE FROM vrac2 
	WHERE
		id = _id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `proc_vrac3_insert_normativ` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `proc_vrac3_insert_normativ`(
	 IN _racunID int(10) unsigned,
	 IN _aid int(10) unsigned,
	 IN _nid int(10) unsigned,
	 IN _kolicina double(14,4)
)
BEGIN
	DECLARE newKolicina double(14,4);
	DECLARE newartnorm_id int(10) unsigned;
	DECLARE done INT DEFAULT FALSE;
	DECLARE counter INT DEFAULT 0;
	
	DECLARE cursorNorm CURSOR FOR 
		SELECT nid, kolicina FROM normativ 
		WHERE aid = _nid;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
	
	OPEN cursorNorm;

read_loop: LOOP
	FETCH cursorNorm INTO newartnorm_id, newKolicina;
	IF done THEN
		LEAVE read_loop;
    END IF;
	SET counter = counter + 1;
	SET newKolicina = newKolicina * _kolicina;
	CALL proc_vrac3_insert_normativ(_racunID, _aid, newartnorm_id, newKolicina);

	END LOOP;
	CLOSE cursorNorm;
	IF counter = 0 THEN
		INSERT INTO vrac3( rid, aid, nid, kol)
		VALUES (_racunID,_aid,_nid,_kolicina);
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `skladNaDan` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `skladNaDan`(
 IN _DatumZS datetime)
begin
drop table if exists izPrometZakljDan;
create table izPrometZakljDan (
 aid int(10),
 naziv varchar(100),
 zaliha double(14,4) default 0,
 primljeno double(14,4) default 0,
 prodano double(14,4) default 0,
 ostatak double(14,4) default 0,
 cijena double(14,4) default 0,
 iznos double(14,4) default 0);
truncate skladnadan;
insert into skladnadan (aid) select id from art;  
update skladnadan s set s.ulaz=(select sum(p2.kolicina) from primk2 p2 left join primk1 p1 on p2.dokid=p1.id  
where date(p1.datumdok) between '2012-12-31' and date(_DatumZS) and aid=s.aid group by aid);
update skladnadan s set s.izlaz=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on r3.rid=r1.id
where r1.datumr between '2013-01-01 00:00:00' and _DatumZS and nid=s.aid group by nid);
update skladnadan set ulaz=0 where ulaz is null;update skladnadan set izlaz=0 where izlaz is null;
update skladnadan set stanje = ulaz-izlaz;
insert into izPrometZakljDan (aid,zaliha) select aid,stanje from skladnadan;
update izPrometZakljDan i set i.prodano=(select sum(r3.kol) from rac3 r3 left join rac1 r1 on 
 r3.rid=r1.id where r1.datumr between '2013-01-01 00:00:00' and _DatumZS and r3.nid=i.aid group by r3.nid);
update izPrometZakljDan set ostatak=0 where ostatak is null;
update izPrometZakljDan set prodano=0 where prodano is null;
update izPrometZakljDan set primljeno=0 where primljeno is null;
update izPrometZakljDan set ostatak=zaliha+primljeno-prodano;
delete from izPrometZakljDan where zaliha=0 and primljeno=0 and prodano=0 and ostatak=0;
update izPrometZakljDan i,art a set i.naziv=a.naziv where i.aid=a.id;
update izPrometZakljDan i,cjenikstavke c set i.cijena=c.punacijena where i.aid=c.aid and c.cid=3;
update izPrometZakljDan set iznos=prodano*cijena;
select * from izPrometZakljDan;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Final view structure for view `t_viz_rac`
--

/*!50001 DROP VIEW IF EXISTS `t_viz_rac`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `t_viz_rac` AS select `a`.`id` AS `id`,`a`.`artsif` AS `artsif`,`a`.`naziv` AS `naziv`,`v`.`punacijena` AS `punacijena`,`s`.`kol` AS `stanje`,`v`.`sid` AS `sid`,`v`.`cid` AS `cid`,`g`.`opis` AS `grupa` from (((`v_a_pcj` `v` left join `art` `a` on((`v`.`aid` = `a`.`id`))) left join `sklad` `s` on(((`a`.`id` = `s`.`aid`) and (`v`.`sid` = `s`.`sid`)))) left join `grupe` `g` on((`a`.`grupaid` = `g`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_a_cjenik`
--

/*!50001 DROP VIEW IF EXISTS `v_a_cjenik`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_a_cjenik` AS select `cs`.`aid` AS `aid`,`cs`.`punacijena` AS `punacijena`,`cs`.`cid` AS `cid`,`c`.`id` AS `id`,`c`.`naziv` AS `naziv` from (`cjenikstavke` `cs` left join `cjenik` `c` on((`cs`.`cid` = `c`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_a_csv`
--

/*!50001 DROP VIEW IF EXISTS `v_a_csv`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_a_csv` AS select `s`.`aid` AS `aid`,`s`.`kol` AS `kol`,`c`.`sid` AS `sid`,`c`.`cid` AS `cid` from (`sklad` `s` left join `cjenikskladisteveza` `c` on((`s`.`sid` = `c`.`sid`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_a_pcj`
--

/*!50001 DROP VIEW IF EXISTS `v_a_pcj`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_a_pcj` AS select `c`.`aid` AS `aid`,`c`.`punacijena` AS `punacijena`,`csv`.`sid` AS `sid`,`csv`.`cid` AS `cid`,`csv`.`isdefault` AS `isdefault` from (`cjenikskladisteveza` `csv` left join `cjenikstavke` `c` on((`csv`.`cid` = `c`.`cid`))) order by `c`.`aid` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_a_prodano`
--

/*!50001 DROP VIEW IF EXISTS `v_a_prodano`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_a_prodano` AS select `rac2`.`aid` AS `aid`,sum(`rac2`.`kol`) AS `prodano` from `rac2` group by `rac2`.`aid` order by sum(`rac2`.`kol`) desc limit 20 */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_d_primk1`
--

/*!50001 DROP VIEW IF EXISTS `v_d_primk1`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_d_primk1` AS select `p`.`id` AS `id`,`p`.`dokbr` AS `dokbr`,`p`.`skladID` AS `skladID`,`c`.`naziv` AS `dobavljac`,`m`.`naziv` AS `naziv`,`p`.`vezadokument` AS `vezadokument`,`p`.`datumdok` AS `datumdok`,`p`.`fiznosstart` AS `fiznosstart`,`p`.`fiznosrabatkn` AS `fiznosrabatkn`,`p`.`fiznosukupno` AS `fiznosukupno`,`p`.`fiznospdv` AS `fiznospdv`,`p`.`ztiznososn` AS `ztiznososn`,`p`.`iznosukupno` AS `iznosukupno` from ((`primk1` `p` left join `matsklad` `m` on((`p`.`skladID` = `m`.`id`))) left join `company` `c` on((`p`.`dobavljacID` = `c`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_d_primk2`
--

/*!50001 DROP VIEW IF EXISTS `v_d_primk2`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_d_primk2` AS select `primk2`.`dokid` AS `dokid`,`primk2`.`aid` AS `aid`,`primk2`.`naziv` AS `naziv`,`primk2`.`jmj` AS `imj`,`primk2`.`kolicina` AS `kolicina` from `primk2` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_d_rac2`
--

/*!50001 DROP VIEW IF EXISTS `v_d_rac2`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_d_rac2` AS select `r`.`rid` AS `rid`,`r`.`aid` AS `aid`,`r`.`naziv` AS `naziv`,`r`.`kol` AS `kol`,`r`.`nc` AS `nc`,`r`.`pc` AS `pc`,`r`.`ukupno` AS `ukupno`,`r`.`bpdv` AS `bpdv`,`r`.`rabatP` AS `rabatP`,`r`.`rabatK` AS `rabatK`,`r`.`ukpcisti` AS `ukpcisti` from `rac2` `r` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_ispis_primk1`
--

/*!50001 DROP VIEW IF EXISTS `v_ispis_primk1`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_ispis_primk1` AS select `p1`.`id` AS `id`,`p1`.`dokbr` AS `p1dokbr`,`p1`.`datumdok` AS `p1datum`,`p1`.`datumv` AS `p1datumvaluta`,`p1`.`vezadokument` AS `p1veza`,`c`.`naziv` AS `dob_naziv`,`c`.`adresa` AS `dob_adresa`,`c`.`mjesto` AS `dob_mjesto`,`c`.`oib` AS `dob_oib`,`p1`.`fiznosukupno` AS `fiznosukupno`,`p1`.`fiznospdv` AS `fiznospdv`,`p1`.`ztiznososn` AS `ztiznososn` from (`primk1` `p1` left join `company` `c` on((`p1`.`dobavljacID` = `c`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_ispis_rac1_1`
--

/*!50001 DROP VIEW IF EXISTS `v_ispis_rac1_1`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_ispis_rac1_1` AS select `r1`.`id` AS `r1id`,`msk`.`naziv` AS `sklad_naziv`,`cje`.`naziv` AS `cjenik_naziv`,`r1`.`br` AS `br`,`r1`.`vrsta` AS `vrsta`,`r1`.`idk` AS `idk`,`r1`.`sum` AS `sum`,`r1`.`bpdv` AS `bpdv`,`r1`.`datumr` AS `datumr`,`r1`.`uid` AS `uid`,`r1`.`obracun` AS `obracun`,`r1`.`stol` AS `stol`,`r1`.`np` AS `np`,`r1`.`storno` AS `storno`,`r1`.`rabatp` AS `rabatp`,`r1`.`rabatk` AS `rabatk`,`r1`.`rsum` AS `rsum`,`r1`.`rbpdv` AS `rbpdv`,`r1`.`storno_datum` AS `storno_datum`,`r1`.`oib` AS `oib`,`r1`.`zki` AS `zki`,`r1`.`sid` AS `sid`,`r1`.`cid` AS `cid`,`r1`.`kasaid` AS `kasaid`,`r1`.`oznPP` AS `oznpp`,`c`.`naziv` AS `kupac_naziv`,`c`.`adresa` AS `kupac_adresa`,`c`.`mjesto` AS `kupac_mjesto`,`c`.`oib` AS `kupac_oib`,`r1f`.`odgovor` AS `jir`,`k`.`alias` AS `nacin_placanja`,`kor`.`PuniNaziv` AS `prodao`,`c`.`countrycode` AS `countrycode`,`eu`.`country` AS `country` from (((((((`rac1` `r1` left join `rac1fiskal` `r1f` on((`r1`.`id` = `r1f`.`racid`))) left join `company` `c` on((`r1`.`idk` = `c`.`id`))) left join `kartice` `k` on((`r1`.`np` = `k`.`id`))) left join `matsklad` `msk` on((`r1`.`sid` = `msk`.`id`))) left join `cjenik` `cje` on((`r1`.`cid` = `cje`.`id`))) left join `korisnik` `kor` on((`r1`.`uid` = `kor`.`id`))) left join `euvatincountry` `eu` on((`c`.`countrycode` = `eu`.`code`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_ispis_rac1_v`
--

/*!50001 DROP VIEW IF EXISTS `v_ispis_rac1_v`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_ispis_rac1_v` AS select `r1`.`id` AS `r1id`,`msk`.`naziv` AS `sklad_naziv`,`cje`.`naziv` AS `cjenik_naziv`,`r1`.`br` AS `br`,`r1`.`vrsta` AS `vrsta`,`r1`.`idk` AS `idk`,`r1`.`sum` AS `sum`,`r1`.`bpdv` AS `bpdv`,`r1`.`datumr` AS `datumr`,`r1`.`uid` AS `uid`,`r1`.`obracun` AS `obracun`,`r1`.`stol` AS `stol`,`r1`.`np` AS `np`,`r1`.`storno` AS `storno`,`r1`.`rabatp` AS `rabatp`,`r1`.`rabatk` AS `rabatk`,`r1`.`rsum` AS `rsum`,`r1`.`rbpdv` AS `rbpdv`,`r1`.`storno_datum` AS `storno_datum`,`r1`.`oib` AS `oib`,'' AS `zki`,`r1`.`sid` AS `sid`,`r1`.`cid` AS `cid`,`r1`.`kasaid` AS `kasaid`,`r1`.`oznpp` AS `oznpp`,`c`.`naziv` AS `kupac_naziv`,`c`.`adresa` AS `kupac_adresa`,`c`.`mjesto` AS `kupac_mjesto`,`c`.`oib` AS `kupac_oib`,'' AS `jir`,`k`.`alias` AS `nacin_placanja`,`kor`.`PuniNaziv` AS `prodao`,`r1`.`datumv` AS `datumv`,`c`.`countrycode` AS `countrycode`,`eu`.`country` AS `country` from ((((((`vrac1` `r1` left join `company` `c` on((`r1`.`idk` = `c`.`id`))) left join `kartice` `k` on((`r1`.`np` = `k`.`id`))) left join `matsklad` `msk` on((`r1`.`sid` = `msk`.`id`))) left join `cjenik` `cje` on((`r1`.`cid` = `cje`.`id`))) left join `korisnik` `kor` on((`r1`.`uid` = `kor`.`id`))) left join `euvatincountry` `eu` on((`c`.`countrycode` = `eu`.`code`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_ispis_vrac1_porezi`
--

/*!50001 DROP VIEW IF EXISTS `v_ispis_vrac1_porezi`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_ispis_vrac1_porezi` AS select `v`.`rid` AS `rid`,`v`.`pid` AS `pid`,`v`.`porez_sifra` AS `porez_sifra`,`v`.`iznos_poreza` AS `iznos_poreza`,`v`.`porez_postotak` AS `porez_postotak`,(`p`.`postotak` * 100) AS `postotak` from (`vrac1porezi` `v` left join `porez` `p` on((`v`.`pid` = `p`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_izv_grupe_gn`
--

/*!50001 DROP VIEW IF EXISTS `v_izv_grupe_gn`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_izv_grupe_gn` AS select `g`.`id` AS `gid`,`g`.`opis` AS `opis`,if(isnull(`gn`.`grupa_id`),`g`.`id`,`gn`.`grupa_id`) AS `gru
pa_id`,if((`g`.`glavna` = 1),`g`.`id`,`gn`.`maticna_grid`) AS `maticna` from (`grupe` `g` left join `grupe_norm` `gn` on((`g`.`id` = `gn`.`maticna_grid`))) where (`g`.`glavna` = 1) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_izv_po_grpid`
--

/*!50001 DROP VIEW IF EXISTS `v_izv_po_grpid`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_izv_po_grpid` AS select `r1`.`id` AS `id`,`r1`.`datumr` AS `datumr`,`r1`.`np` AS `np`,`r1`.`vrsta` AS `vrsta`,`rp`.`aid` AS `aid`,`a`.`grupaid` AS `grupaid`,`rp`.`porez_opis` AS `porez`,sum(`r2`.`ukupno`) AS `osnovica`,sum(`rp`.`iznos_poreza`) AS `iznos` from (((`rac2` `r2` left join `rac2porezi` `rp` on(((`r2`.`rid` = `rp`.`rid`) and (`r2`.`aid` = `rp`.`aid`)))) left join `art` `a` on((`r2`.`aid` = `a`.`id`))) left join `rac1` `r1` on((`rp`.`rid` = `r1`.`id`))) group by `r1`.`id`,`rp`.`aid`,`rp`.`pid` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_kasa_pregleddok`
--

/*!50001 DROP VIEW IF EXISTS `v_kasa_pregleddok`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_kasa_pregleddok` AS select `r1`.`id` AS `id`,`r1`.`br` AS `br`,concat(`r1`.`br`,'/',`r1`.`oznPP`,'/',`r1`.`kasaid`) AS `broj_racuna`,`r1`.`vrsta` AS `vrsta`,`r1`.`datumr` AS `datumr`,`r1`.`sum` AS `sum`,`r1`.`bpdv` AS `bpdv`,`r1`.`rabatp` AS `rabatp`,`r1`.`rabatk` AS `rabatk`,`np`.`alias` AS `alias`,`k`.`naziv` AS `prodavac`,concat(`c`.`naziv`,' ',`c`.`adresa`,',Oib:',`c`.`oib`) AS `kupac`,`r1`.`stol` AS `stol`,`r1`.`storno` AS `storno`,`r1`.`rsum` AS `rsum`,`r1`.`rbpdv` AS `rbpdv`,`r1`.`zki` AS `zki`,`r1`.`sid` AS `sid`,`r1`.`cid` AS `cid`,`r1`.`oznPP` AS `oznpp`,`r1`.`kasaid` AS `kasaid`,`r1`.`storno_datum` AS `storno_datum`,`r1`.`uid` AS `uid` from (((`rac1` `r1` left join `company` `c` on((`r1`.`idk` = `c`.`id`))) left join `korisnik` `k` on((`r1`.`uid` = `k`.`id`))) left join `kartice` `np` on((`r1`.`np` = `np`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_xml_raczahtjev`
--

/*!50001 DROP VIEW IF EXISTS `v_xml_raczahtjev`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_xml_raczahtjev` AS select `r1`.`id` AS `id`,`r1`.`br` AS `br`,`r1`.`vrsta` AS `vrsta`,`r1`.`idk` AS `idk`,`r1`.`sum` AS `sum`,`r1`.`bpdv` AS `bpdv`,`r1`.`datumr` AS `datumr`,`r1`.`uid` AS `uid`,`r1`.`obracun` AS `obracun`,`r1`.`stol` AS `stol`,`r1`.`np` AS `np`,`r1`.`storno` AS `storno`,`r1`.`rabatp` AS `rabatp`,`r1`.`rabatk` AS `rabatk`,`r1`.`rsum` AS `rsum`,`r1`.`rbpdv` AS `rbpdv`,`r1`.`storno_datum` AS `storno_datum`,`r1`.`oib` AS `oib`,`r1`.`zki` AS `zki`,`r1`.`sid` AS `sid`,`r1`.`cid` AS `cid`,ifnull(`r1p`.`porez_sifra`,0) AS `porez_sifra`,ifnull(sum(`r1p`.`iznos_poreza`),0) AS `iz` from (`rac1` `r1` left join `rac1porezi` `r1p` on((`r1`.`id` = `r1p`.`rid`))) group by `r1`.`id` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_xml_raczahtjev_porez`
--

/*!50001 DROP VIEW IF EXISTS `v_xml_raczahtjev_porez`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_xml_raczahtjev_porez` AS select `r2`.`rid` AS `rid`,`r2`.`aid` AS `aid`,`r2`.`bpdv` AS `osnovica`,sum(`r2p`.`iznos_poreza`) AS `iznos`,`r2p`.`pid` AS `pid`,(`p`.`postotak` * 100) AS `postotak`,`p`.`poreztip` AS `tip` from ((`rac2` `r2` left join `rac2porezi` `r2p` on(((`r2`.`rid` = `r2p`.`rid`) and (`r2`.`aid` = `r2p`.`aid`)))) left join `porez` `p` on((`r2p`.`pid` = `p`.`id`))) group by `r2`.`rid`,`r2p`.`pid` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `view_stanje_skladista`
--

/*!50001 DROP VIEW IF EXISTS `view_stanje_skladista`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `view_stanje_skladista` AS select `a`.`artsif` AS `artsif`,`a`.`naziv` AS `naziv`,ifnull(`s`.`kol`,0) AS `stanje`,`s`.`sid` AS `sid`,`m`.`naziv` AS `skladiste` from ((`sklad` `s` left join `art` `a` on((`s`.`aid` = `a`.`id`))) left join `matsklad` `m` on((`s`.`sid` = `m`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viz_porezi`
--

/*!50001 DROP VIEW IF EXISTS `viz_porezi`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viz_porezi` AS select `a`.`aid` AS `aid`,`p`.`postotak` AS `postotak`,((`p`.`postotak` * 100) / (100 + (select sum((`p`.`postotak` * 100)) from (`artiklporez` `ap` left join `porez` `p` on((`ap`.`porezid` = `p`.`id`))) where (`ap`.`aid` = `a`.`aid`)))) AS `koef`,`p`.`id` AS `id`,`p`.`sifra` AS `sifra` from (`artiklporez` `a` left join `porez` `p` on((`a`.`porezid` = `p`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viz_rac`
--

/*!50001 DROP VIEW IF EXISTS `viz_rac`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viz_rac` AS select `a`.`id` AS `id`,`a`.`artsif` AS `artsif`,`a`.`naziv` AS `naziv`,`v`.`punacijena` AS `PC`,`s`.`kol` AS `stanje`,`v`.`sid` AS `sid`,`v`.`cid` AS `cid`,`a`.`grupaid` AS `grupaid`,`g`.`opis` AS `opis`,`g`.`ispis_kuhinja` AS `ispis_kuhinja`,`a`.`povratnanaknada` AS `povratnanaknada` from (((`v_a_pcj` `v` left join `art` `a` on((`v`.`aid` = `a`.`id`))) left join `sklad` `s` on(((`a`.`id` = `s`.`aid`) and (`v`.`sid` = `s`.`sid`)))) left join `grupe` `g` on((`a`.`grupaid` = `g`.`id`))) where (`a`.`vidljiv` = '1') */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viz_rac_top20`
--

/*!50001 DROP VIEW IF EXISTS `viz_rac_top20`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viz_rac_top20` AS select `a`.`id` AS `id`,`a`.`artsif` AS `artsif`,`a`.`naziv` AS `naziv`,`v`.`punacijena` AS `PC`,`vp`.`prodano` AS `prodano`,`v`.`sid` AS `sid`,`v`.`cid` AS `cid`,`a`.`grupaid` AS `grupaid`,`g`.`opis` AS `opis`,`g`.`ispis_kuhinja` AS `ispis_kuhinja`,`a`.`povratnanaknada` AS `povratnanaknada` from (((`v_a_prodano` `vp` left join `v_a_pcj` `v` on((`vp`.`aid` = `v`.`aid`))) left join `art` `a` on((`v`.`aid` = `a`.`id`))) left join `grupe` `g` on((`a`.`grupaid` = `g`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-10-10 11:25:34
