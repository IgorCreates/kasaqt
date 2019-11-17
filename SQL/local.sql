-- MySQL dump 10.13  Distrib 5.7.15, for Linux (x86_64)
--
-- Host: localhost    Database: local
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
-- Table structure for table `baze`
--

DROP TABLE IF EXISTS `baze`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baze` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `host` varchar(250) DEFAULT NULL,
  `username` varchar(250) DEFAULT NULL,
  `password` varchar(250) DEFAULT NULL,
  `baza` varchar(250) DEFAULT NULL,
  `opis` varchar(250) DEFAULT NULL,
  `tip` varchar(250) DEFAULT NULL,
  `izabrana` enum('0','1') DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `del_stol`
--

DROP TABLE IF EXISTS `del_stol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `del_stol` (
  `id` int(10) DEFAULT '0',
  `dok` int(10) DEFAULT NULL,
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
  `narudzba` int(1) DEFAULT NULL,
  `narucio_uid` int(10) DEFAULT NULL,
  `narid` int(10) DEFAULT '0',
  `datebrisano` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `del_stol4`
--

DROP TABLE IF EXISTS `del_stol4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `del_stol4` (
  `id` int(10) NOT NULL,
  `dok` int(10) DEFAULT NULL,
  `aid` int(10) DEFAULT NULL,
  `pid` int(10) DEFAULT NULL,
  `koeficijent_poreza` double(24,8) DEFAULT NULL,
  `iznos_poreza` double(14,4) DEFAULT NULL,
  `porez_sifra` varchar(250) DEFAULT NULL,
  `porez_postotak` double(14,7) DEFAULT NULL,
  `r2id` int(10) DEFAULT NULL,
  `iznos_porezaz` double(18,6) DEFAULT NULL,
  `osnovica` double(20,4) DEFAULT '0.0000',
  `narid` int(10) DEFAULT '0',
  `datebrisano` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `fiskaloznpp`
--

DROP TABLE IF EXISTS `fiskaloznpp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fiskaloznpp` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `oznpp` varchar(20) DEFAULT NULL,
  `datumstart` date DEFAULT NULL,
  `a_ulica` varchar(250) DEFAULT NULL,
  `a_broj` varchar(10) DEFAULT NULL,
  `a_dodatakbroju` varchar(10) DEFAULT NULL,
  `a_brojposte` varchar(10) DEFAULT NULL,
  `a_naselje` varchar(250) DEFAULT NULL,
  `a_opcina` varchar(250) DEFAULT NULL,
  `a_ostali_tipovi` longtext,
  `radno_vrijeme` varchar(250) DEFAULT NULL,
  `oznaka_zatvaranja` char(1) DEFAULT NULL,
  `specificna_namjena` varchar(250) DEFAULT NULL,
  `adresni_podatak` enum('A','O') DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ispkuhinja`
--

DROP TABLE IF EXISTS `ispkuhinja`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ispkuhinja` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `dok` int(10) DEFAULT NULL,
  `aid` int(10) DEFAULT NULL,
  `kol` double(14,4) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `isppostavke`
--

DROP TABLE IF EXISTS `isppostavke`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `isppostavke` (
  `sto` varchar(250) DEFAULT NULL,
  `value1` longtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
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
-- Table structure for table `log_rac2`
--

DROP TABLE IF EXISTS `log_rac2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `log_rac2` (
  `id` int(10) NOT NULL DEFAULT '0',
  `dok` int(10) DEFAULT NULL,
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
  `narudzba` int(1) DEFAULT '0',
  `narucio_uid` int(10) DEFAULT NULL,
  `narid` int(10) DEFAULT '0',
  `datebrisano` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `log_rac4`
--

DROP TABLE IF EXISTS `log_rac4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `log_rac4` (
  `id` int(10) NOT NULL DEFAULT '0',
  `dok` int(10) DEFAULT NULL,
  `aid` int(10) DEFAULT NULL,
  `pid` int(10) DEFAULT NULL,
  `koeficijent_poreza` double(24,8) DEFAULT NULL,
  `iznos_poreza` double(14,4) DEFAULT NULL,
  `porez_sifra` varchar(250) DEFAULT NULL,
  `porez_postotak` double(14,7) DEFAULT NULL,
  `r2id` int(10) DEFAULT NULL,
  `iznos_porezaz` double(18,6) DEFAULT NULL,
  `osnovica` double(20,4) DEFAULT '0.0000',
  `narid` int(10) DEFAULT '0',
  `dateobrisano` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `opcije`
--

DROP TABLE IF EXISTS `opcije`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `opcije` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `Sto` varchar(250) DEFAULT NULL,
  `Sifra` varchar(250) DEFAULT NULL,
  `Opis` varchar(250) DEFAULT NULL,
  `Value1` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=171 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `opcije_naruci`
--

DROP TABLE IF EXISTS `opcije_naruci`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `opcije_naruci` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `sank_opis` varchar(20) DEFAULT NULL,
  `host` varchar(100) DEFAULT NULL,
  `baza` varchar(100) DEFAULT NULL,
  `user` varchar(100) DEFAULT NULL,
  `pass` varchar(100) DEFAULT NULL,
  `kasaid` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac2`
--

DROP TABLE IF EXISTS `rac2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac2` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `dok` int(10) DEFAULT NULL,
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
  `narudzba` int(1) DEFAULT NULL,
  `narucio_uid` int(10) DEFAULT NULL,
  `narid` int(10) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=358 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rac4`
--

DROP TABLE IF EXISTS `rac4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rac4` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `dok` int(10) DEFAULT NULL,
  `aid` int(10) DEFAULT NULL,
  `pid` int(10) DEFAULT NULL,
  `koeficijent_poreza` double(24,8) DEFAULT NULL,
  `iznos_poreza` double(14,4) DEFAULT NULL,
  `porez_sifra` varchar(250) DEFAULT NULL,
  `porez_postotak` double(14,7) DEFAULT NULL,
  `r2id` int(10) DEFAULT NULL,
  `iznos_porezaz` double(18,6) DEFAULT NULL,
  `osnovica` double(20,4) DEFAULT '0.0000',
  `narid` int(10) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=362 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping routines for database 'local'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-10-10 11:25:46
