-- MySQL dump 10.13  Distrib 5.5.34, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: fqscan_v03
-- ------------------------------------------------------
-- Server version	5.5.34-0ubuntu0.13.04.1

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
-- Table structure for table `control`
--

DROP TABLE IF EXISTS `control`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `control` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `chart_button_bands` tinyint(1) NOT NULL,
  `chart_button_limits` tinyint(1) NOT NULL,
  `chart_button_max` tinyint(1) NOT NULL,
  `chart_button_cur` tinyint(1) NOT NULL,
  `chart_button_x_coord` tinyint(1) NOT NULL,
  `chart_button_y_coord` tinyint(1) NOT NULL,
  `chart_button_x_grid` tinyint(1) NOT NULL,
  `chart_button_y_grid` tinyint(1) NOT NULL,
  `params_freq_range_start` float NOT NULL,
  `params_freq_range_end` float NOT NULL,
  `params_coarse_bw` float NOT NULL,
  `params_threshold` float NOT NULL,
  `params_freq_corr` int(11) NOT NULL,
  `params_freq_shift` float NOT NULL,
  `params_average_count` int(11) NOT NULL,
  `params_fine_bw` float NOT NULL,
  `params_fft_width` int(11) NOT NULL,
  `params_gain` float NOT NULL,
  `params_filter` varchar(32) NOT NULL,
  `params_sample_rate` int(11) NOT NULL,
  `params_spread` int(11) NOT NULL,
  `params_step` float NOT NULL,
  `params_if_gain` float NOT NULL,
  `params_p_time` float NOT NULL,
  `params_filter_a1` float DEFAULT NULL,
  `params_filter_a2` float DEFAULT NULL,
  `params_filter_a3` float DEFAULT NULL,
  `params_filter_a4` float DEFAULT NULL,
  `params_filter_a5` float DEFAULT NULL,
  `params_band_info` varchar(32) NOT NULL,
  `opers_button_basic` tinyint(1) NOT NULL,
  `opers_button_advanced` tinyint(1) NOT NULL,
  `opers_button_clear` tinyint(1) NOT NULL,
  `opers_button_reset` tinyint(1) NOT NULL,
  `opers_button_update` tinyint(1) NOT NULL,
  `opers_button_start` tinyint(1) NOT NULL,
  `opers_button_stop` tinyint(1) NOT NULL,
  `res_chart_width` int(11) NOT NULL,
  `res_chart_height` int(11) NOT NULL,
  `interface_button_header` tinyint(1) NOT NULL,
  `interface_button_footer` tinyint(1) NOT NULL,
  `interface_button_full` tinyint(1) NOT NULL,
  `changed_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `processed` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `control`
--

LOCK TABLES `control` WRITE;
/*!40000 ALTER TABLE `control` DISABLE KEYS */;
/*!40000 ALTER TABLE `control` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `filter_types`
--

DROP TABLE IF EXISTS `filter_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `filter_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `filter_type` varchar(32) NOT NULL,
  `description` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `filter_types`
--

LOCK TABLES `filter_types` WRITE;
/*!40000 ALTER TABLE `filter_types` DISABLE KEYS */;
INSERT INTO `filter_types` VALUES (1,'HR','High-Resolution Filter'),(2,'MR','Medium-Resolution Filter'),(3,'LR','Low-Resolution Filter');
/*!40000 ALTER TABLE `filter_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `filters`
--

DROP TABLE IF EXISTS `filters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `filters` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `filter` varchar(64) DEFAULT NULL,
  `filter_types_id` int(11) NOT NULL DEFAULT '1',
  `def` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `filter_types_id` (`filter_types_id`),
  CONSTRAINT `filters_ibfk_1` FOREIGN KEY (`filter_types_id`) REFERENCES `filter_types` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `filters`
--

LOCK TABLES `filters` WRITE;
/*!40000 ALTER TABLE `filters` DISABLE KEYS */;
INSERT INTO `filters` VALUES (1,'Rectangular',1,0),(2,'Bartlett',2,0),(3,'Bartlett-Hann',2,0),(4,'Blackman',2,0),(5,'Cosine',2,0),(6,'Dolph-Chebyshev',2,1),(7,'Gauss',2,0),(8,'Hamming',2,0),(9,'Hann',2,0),(10,'Hann-Poisson',2,0),(11,'Kaiser',2,0),(12,'Planck-Bessel',2,0),(13,'Planck-taper',2,0),(14,'Saramaki',2,0),(15,'Lanczos',2,0),(16,'Triangular',2,0),(17,'Tukey',2,0),(18,'Ultraspherical',2,0),(19,'Blackman-Harris',3,0),(20,'Blackman-Nuttall',3,0),(21,'Flat-Top',3,0),(22,'Generic-3rd',3,0),(23,'Generic-4th',3,0),(24,'Nuttall',3,0);
/*!40000 ALTER TABLE `filters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scan_data`
--

DROP TABLE IF EXISTS `scan_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scan_data` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `setup_id` int(11) NOT NULL,
  `freq` double NOT NULL,
  `width` double NOT NULL,
  `power` double NOT NULL,
  `static_diff` double NOT NULL,
  `range_freq_start` double NOT NULL,
  `range_freq_end` double NOT NULL,
  `global_ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `scan_ts` time NOT NULL,
  `count` int(11) NOT NULL DEFAULT '0',
  `completed` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `setup_id` (`setup_id`),
  CONSTRAINT `scan_data_ibfk_1` FOREIGN KEY (`setup_id`) REFERENCES `setup` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scan_data`
--

LOCK TABLES `scan_data` WRITE;
/*!40000 ALTER TABLE `scan_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `scan_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `setup`
--

DROP TABLE IF EXISTS `setup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `setup` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `average` int(11) NOT NULL DEFAULT '1000',
  `coarse_bandwidth` double NOT NULL DEFAULT '200000',
  `fine_bandwidth` double NOT NULL DEFAULT '25000',
  `spread` double NOT NULL DEFAULT '50000',
  `threshold` double NOT NULL DEFAULT '3',
  `freq_start` double NOT NULL DEFAULT '87000000',
  `freq_end` double NOT NULL DEFAULT '108000000',
  `sample_rate` double NOT NULL DEFAULT '2000000',
  `fft_width` double NOT NULL DEFAULT '1000',
  `step` double NOT NULL DEFAULT '1000000',
  `ptime` double NOT NULL DEFAULT '0.1',
  `freq_correction` double NOT NULL,
  `gain` double NOT NULL,
  `if_gain` double NOT NULL,
  `freq_shift` double NOT NULL,
  `filters_id` int(11) NOT NULL DEFAULT '1',
  `filter_arg1` double DEFAULT NULL,
  `filter_arg2` double DEFAULT NULL,
  `filter_arg3` double DEFAULT NULL,
  `filter_arg4` double DEFAULT NULL,
  `filter_arg5` double DEFAULT NULL,
  `filter_args` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `filters_id` (`filters_id`),
  CONSTRAINT `setup_ibfk_1` FOREIGN KEY (`filters_id`) REFERENCES `filters` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `setup`
--

LOCK TABLES `setup` WRITE;
/*!40000 ALTER TABLE `setup` DISABLE KEYS */;
/*!40000 ALTER TABLE `setup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `state`
--

DROP TABLE IF EXISTS `state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `state` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `setup_id` int(11) NOT NULL,
  `running` tinyint(1) NOT NULL DEFAULT '0',
  `freq_range_start` float DEFAULT NULL,
  `freq_range_end` float DEFAULT NULL,
  `xo` double DEFAULT NULL,
  `xoo` double DEFAULT NULL,
  `yo` double DEFAULT NULL,
  `yoo` double DEFAULT NULL,
  `scan_ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `setup_id` (`setup_id`),
  CONSTRAINT `state_ibfk_1` FOREIGN KEY (`setup_id`) REFERENCES `setup` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `state`
--

LOCK TABLES `state` WRITE;
/*!40000 ALTER TABLE `state` DISABLE KEYS */;
/*!40000 ALTER TABLE `state` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-04-26  4:05:40
