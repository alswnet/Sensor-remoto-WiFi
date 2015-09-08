-- MySQL dump 10.13  Distrib 5.1.73, for redhat-linux-gnu (x86_64)
--
-- Host: localhost    Database: sees
-- ------------------------------------------------------
-- Server version	5.1.73

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
-- Table structure for table `app_config`
--

DROP TABLE IF EXISTS `app_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `app_config` (
  `userid` int(11) DEFAULT NULL,
  `data` text
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `app_config`
--

LOCK TABLES `app_config` WRITE;
/*!40000 ALTER TABLE `app_config` DISABLE KEYS */;
/*!40000 ALTER TABLE `app_config` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `applist`
--

DROP TABLE IF EXISTS `applist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `applist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT NULL,
  `name` text,
  `list` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `applist`
--

LOCK TABLES `applist` WRITE;
/*!40000 ALTER TABLE `applist` DISABLE KEYS */;
/*!40000 ALTER TABLE `applist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dashboard`
--

DROP TABLE IF EXISTS `dashboard`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dashboard` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT NULL,
  `content` text,
  `height` int(11) DEFAULT NULL,
  `name` varchar(30) DEFAULT 'no name',
  `alias` varchar(10) DEFAULT NULL,
  `description` varchar(255) DEFAULT 'no description',
  `main` tinyint(1) DEFAULT NULL,
  `public` tinyint(1) DEFAULT NULL,
  `published` tinyint(1) DEFAULT NULL,
  `showdescription` tinyint(1) DEFAULT NULL,
  `backgroundcolor` varchar(6) DEFAULT 'EDF7FC',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dashboard`
--

LOCK TABLES `dashboard` WRITE;
/*!40000 ALTER TABLE `dashboard` DISABLE KEYS */;
INSERT INTO `dashboard` VALUES (3,1,'<div id=\"11\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 840px; left: 20px; width: 1140px; height: 440px;\" mid=\"5\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=5\" style=\"width: 1140px; height: 440px;\"></iframe></div><div id=\"12\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 440px; left: 20px; width: 580px; height: 380px;\" mid=\"10\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=10\" style=\"width: 580px; height: 380px;\"></iframe></div><div id=\"13\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 280px; left: 600px; width: 140px; height: 140px;\" feed=\"CM-Diszada-Camara-Fria\" max=\"60\" scale=\"\" units=\"C\" offset=\"\" type=\"7\" graduations=\"1\"><canvas id=\"can-13\" width=\"140\" height=\"140\"></canvas></div><div id=\"14\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 80px; left: 1000px; width: 140px; height: 140px;\" feed=\"mini-Pro-2:Temperatura\" max=\"30\" scale=\"\" units=\"ÂºC\" offset=\"\" type=\"7\" graduations=\"1\"><canvas id=\"can-14\" width=\"140\" height=\"140\"></canvas></div><div id=\"15\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 280px; left: 780px; width: 140px; height: 140px;\" feed=\"DZ-Rpi-Temp\" max=\"90\" scale=\"\" units=\"ÂºC\" offset=\"\" type=\"8\" graduations=\"1\"><canvas id=\"can-15\" width=\"140\" height=\"140\"></canvas></div><div id=\"30\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 0px; left: 100px; width: 960px; height: 60px;\">MEDICION DE TEMPERATURA PRE-CAMARA Y CAMARA FRIA</div><div id=\"31\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 440px; left: 600px; width: 560px; height: 380px;\" mid=\"11\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=11\" style=\"width: 560px; height: 380px;\"></iframe></div><div id=\"34\" class=\"realtime\" style=\"position: absolute; margin: 0px; top: 60px; left: 580px; width: 420px; height: 180px;\" feedid=\"38\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/realtime?embed=1&amp;feedid=38\" style=\"width: 420px; height: 180px;\"></iframe></div><div id=\"36\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 240px; left: 600px; width: 140px; height: 60px;\">pre-Camara</div><div id=\"37\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 240px; left: 760px; width: 180px; height: 60px;\">Camara Fria</div><div id=\"38\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 40px; left: 1000px; width: 140px; height: 60px;\">Refri D1</div><div id=\"39\" class=\"bargraph\" style=\"position: absolute; margin: 0px; top: 60px; left: 20px; width: 560px; height: 380px;\" feedid=\"38\" colour=\"33ffff\" interval=\"\" units=\"ÂºC\" dp=\"\" scale=\"\" delta=\"\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/bargraph?embed=1&amp;feedid=38&amp;colour=33ffff&amp;interval=&amp;units=ÂºC&amp;dp=&amp;scale=&amp;delta=\" style=\"width: 560px; height: 380px;\"></iframe></div><div id=\"40\" class=\"feedvalue\" style=\"position: absolute; margin: 0px; top: 280px; left: 1040px; width: 80px; height: 60px;\" feedname=\"max-temp-Rpi\" units=\"ÂºC\" decimals=\"2\">11.50ÂºC</div><div id=\"41\" class=\"feedvalue\" style=\"position: absolute; margin: 0px; top: 360px; left: 1040px; width: 80px; height: 60px;\" feedname=\"min-temp-Rpi-2\" units=\"ÂºC\" decimals=\"2\">6.81ÂºC</div><div id=\"42\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 280px; left: 920px; width: 140px; height: 60px;\">MAX</div><div id=\"43\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 360px; left: 940px; width: 120px; height: 60px;\">MIN</div>',1280,'Demo Diszasa','Demo2','DEMO SEES',1,1,1,0,'ffffff'),(2,1,'<div id=\"1\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 60px; left: 820px; width: 140px; height: 140px;\" feed=\"Demo_Energia:P_TOT\" max=\"500\" scale=\"\" units=\"W\" offset=\"\" type=\"6\" graduations=\"1\"><canvas id=\"can-1\" width=\"140\" height=\"140\"></canvas></div><div id=\"4\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 60px; left: 980px; width: 140px; height: 140px;\" feed=\"Demo_Energia:PF1\" max=\"1\" scale=\"\" units=\"\" offset=\"\" type=\"0\" graduations=\"1\"><canvas id=\"can-4\" width=\"140\" height=\"140\"></canvas></div><div id=\"5\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 220px; left: 820px; width: 140px; height: 140px;\" feed=\"Demo_Energia:A1\" max=\"200\" scale=\"\" units=\"A\" offset=\"\" type=\"0\" graduations=\"1\"><canvas id=\"can-5\" width=\"140\" height=\"140\"></canvas></div><div id=\"6\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 220px; left: 980px; width: 140px; height: 140px;\" feed=\"Demo_Energia:V1\" max=\"240\" scale=\"\" units=\"V\" offset=\"\" type=\"0\" graduations=\"1\"><canvas id=\"can-6\" width=\"140\" height=\"140\"></canvas></div><div id=\"7\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 60px; left: 20px; width: 740px; height: 320px;\" mid=\"4\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=4\" style=\"width: 740px; height: 320px;\"></iframe></div><div id=\"8\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 400px; left: 20px; width: 620px; height: 180px;\" mid=\"2\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=2\" style=\"width: 620px; height: 180px;\"></iframe></div><div id=\"9\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 600px; left: 20px; width: 620px; height: 180px;\" mid=\"3\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=3\" style=\"width: 620px; height: 180px;\"></iframe></div><div id=\"10\" class=\"bargraph\" style=\"position: absolute; margin: 0px; top: 400px; left: 660px; width: 480px; height: 380px;\" feedid=\"8\" colour=\"008000\" interval=\"d\" units=\"KW/h\" dp=\"\" scale=\"\" delta=\"\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/bargraph?embed=1&amp;feedid=8&amp;colour=008000&amp;interval=d&amp;units=KW/h&amp;dp=&amp;scale=&amp;delta=\" style=\"width: 480px; height: 380px;\"></iframe></div><div id=\"11\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 880px; left: 20px; width: 680px; height: 460px;\" mid=\"5\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=5\" style=\"width: 680px; height: 460px;\"></iframe></div><div id=\"12\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 880px; left: 720px; width: 420px; height: 300px;\" mid=\"6\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=6\" style=\"width: 420px; height: 300px;\"></iframe></div><div id=\"13\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 1220px; left: 720px; width: 120px; height: 120px;\" feed=\"mini-Pro-2:Temperatura\" max=\"60\" scale=\"\" units=\"C\" offset=\"\" type=\"7\" graduations=\"1\"><canvas id=\"can-13\" width=\"120\" height=\"120\"></canvas></div><div id=\"14\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 1220px; left: 860px; width: 120px; height: 120px;\" feed=\"icon3:Humidity\" max=\"100\" scale=\"\" units=\"%\" offset=\"\" type=\"10\" graduations=\"1\"><canvas id=\"can-14\" width=\"120\" height=\"120\"></canvas></div><div id=\"15\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 1220px; left: 1000px; width: 120px; height: 120px;\" feed=\"mini-Pro-1:Humedad\" max=\"100\" scale=\"\" units=\"%\" offset=\"\" type=\"10\" graduations=\"1\"><canvas id=\"can-15\" width=\"120\" height=\"120\"></canvas></div><div id=\"16\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 1340px; left: 20px; width: 840px; height: 300px;\" mid=\"7\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=7\" style=\"width: 840px; height: 300px;\"></iframe></div><div id=\"18\" class=\"bar\" style=\"position: absolute; margin: 0px; top: 1340px; left: 900px; width: 100px; height: 300px;\" feed=\"icon3:Temperatura\" max=\"60\" scale=\"\" units=\"C\" offset=\"\" colour=\"33ffff\" graduations=\"1\" gradnumber=\"5\"><canvas id=\"can-18\" width=\"100\" height=\"300\"></canvas></div><div id=\"19\" class=\"bar\" style=\"position: absolute; margin: 0px; top: 1340px; left: 1020px; width: 100px; height: 300px;\" feed=\"mini-Pro-1:Temperatura\" max=\"60\" scale=\"\" units=\"C\" offset=\"\" colour=\"00ffff\" graduations=\"1\" gradnumber=\"5\"><canvas id=\"can-19\" width=\"100\" height=\"300\"></canvas></div><div id=\"20\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 1180px; left: 720px; width: 120px; height: 40px;\">C/R</div><div id=\"21\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 1180px; left: 860px; width: 120px; height: 40px;\">MR</div><div id=\"22\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 1180px; left: 1000px; width: 120px; height: 40px;\">DC</div><div id=\"23\" class=\"multigraph\" style=\"position: absolute; margin: 0px; top: 1740px; left: 520px; width: 620px; height: 220px;\" mid=\"8\"><iframe frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" src=\"http://50.116.76.54/sees/vis/multigraph?embed=1&amp;mid=8\" style=\"width: 620px; height: 220px;\"></iframe></div><div id=\"24\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 1760px; left: 40px; width: 140px; height: 140px;\" feed=\"mini-Pro-2:ppm\" max=\"2500\" scale=\"PPM\" units=\"\" offset=\"\" type=\"10\" graduations=\"1\"><canvas id=\"can-24\" width=\"140\" height=\"140\"></canvas></div><div id=\"25\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 1760px; left: 200px; width: 140px; height: 140px;\" feed=\"icon3:ppm\" max=\"2500\" scale=\"\" units=\"\" offset=\"\" type=\"10\" graduations=\"1\"><canvas id=\"can-25\" width=\"140\" height=\"140\"></canvas></div><div id=\"26\" class=\"dial\" style=\"position: absolute; margin: 0px; top: 1760px; left: 360px; width: 140px; height: 140px;\" feed=\"mini-Pro-1:ppm\" max=\"2500\" scale=\"\" units=\"\" offset=\"\" type=\"10\" graduations=\"1\"><canvas id=\"can-26\" width=\"140\" height=\"140\"></canvas></div><div id=\"27\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 1900px; left: 60px; width: 100px; height: 60px;\">C/R SMOKE</div><div id=\"28\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 1900px; left: 220px; width: 100px; height: 60px;\">MR SMOKE</div><div id=\"29\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 1900px; left: 380px; width: 100px; height: 60px;\">DC SMOKE</div><div id=\"30\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 0px; left: 100px; width: 960px; height: 60px;\">MEDICION DE ENERGIA</div><div id=\"31\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 800px; left: 100px; width: 960px; height: 60px;\">MEDICION TEMPERATURA Y HUMEDAD\n</div><div id=\"32\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 880px; left: 40px; width: 680px; height: 60px;\">EFICIENCIA EÂº (TÂº vrs kW/h)</div><div id=\"33\" class=\"heading-center\" style=\"position: absolute; margin: 0px; top: 1660px; left: 100px; width: 960px; height: 60px;\">MEDICION DE CALIDAD DE AIRE Y SENSOR DE HUMO</div>',1960,'E T HUM PPM SMOKE','DEMO','DEMO SEES',0,1,1,0,'ffffff');
/*!40000 ALTER TABLE `dashboard` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `energy`
--

DROP TABLE IF EXISTS `energy`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `energy` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) NOT NULL,
  `tag` text,
  `year` int(11) DEFAULT NULL,
  `data` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `energy`
--

LOCK TABLES `energy` WRITE;
/*!40000 ALTER TABLE `energy` DISABLE KEYS */;
/*!40000 ALTER TABLE `energy` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `event`
--

DROP TABLE IF EXISTS `event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `event` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT NULL,
  `eventfeed` int(11) DEFAULT NULL,
  `eventtype` int(11) DEFAULT NULL,
  `eventvalue` float DEFAULT NULL,
  `action` int(11) DEFAULT NULL,
  `setfeed` int(11) DEFAULT NULL,
  `setemail` text,
  `setvalue` float DEFAULT NULL,
  `callcurl` text,
  `lasttime` int(11) DEFAULT NULL,
  `mutetime` int(11) DEFAULT NULL,
  `priority` int(3) DEFAULT NULL,
  `message` text,
  `disabled` int(1) DEFAULT NULL,
  `mqtttopic` text,
  `mqttqos` int(3) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `event`
--

LOCK TABLES `event` WRITE;
/*!40000 ALTER TABLE `event` DISABLE KEYS */;
INSERT INTO `event` VALUES (2,1,38,3,0,0,2,'luiscd7@gmail.com',0,'',0,0,-2,'{feed} is {value}',0,'emoncms/event/{feed}',0),(3,1,38,0,12,0,2,'luiscd7@gmai.com',0,'',0,0,-2,'{feed} is {value}',0,'emoncms/event/{feed}',0);
/*!40000 ALTER TABLE `event` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `event_settings`
--

DROP TABLE IF EXISTS `event_settings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `event_settings` (
  `userid` int(11) DEFAULT NULL,
  `smtpserver` varchar(60) DEFAULT NULL,
  `smtpuser` varchar(60) DEFAULT NULL,
  `smtppassword` varchar(128) DEFAULT NULL,
  `smtpport` varchar(3) DEFAULT NULL,
  `consumerkey` varchar(60) DEFAULT NULL,
  `consumersecret` varchar(60) DEFAULT NULL,
  `usertoken` varchar(60) DEFAULT NULL,
  `usersecret` varchar(60) DEFAULT NULL,
  `prowlkey` varchar(60) DEFAULT NULL,
  `nmakey` varchar(60) DEFAULT NULL,
  `mqttbrokerip` varchar(60) DEFAULT NULL,
  `mqttbrokerport` varchar(5) DEFAULT NULL,
  `mqttusername` varchar(60) DEFAULT NULL,
  `mqttpassword` varchar(60) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `event_settings`
--

LOCK TABLES `event_settings` WRITE;
/*!40000 ALTER TABLE `event_settings` DISABLE KEYS */;
INSERT INTO `event_settings` VALUES (1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `event_settings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `feeds`
--

DROP TABLE IF EXISTS `feeds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `feeds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text,
  `userid` int(11) DEFAULT NULL,
  `tag` text,
  `time` datetime DEFAULT NULL,
  `value` float DEFAULT NULL,
  `datatype` int(11) NOT NULL,
  `public` tinyint(1) DEFAULT NULL,
  `size` int(11) DEFAULT NULL,
  `engine` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=73 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `feeds`
--

LOCK TABLES `feeds` WRITE;
/*!40000 ALTER TABLE `feeds` DISABLE KEYS */;
INSERT INTO `feeds` VALUES (11,'mini Pro 1:Humedad',1,'Node:000018FE34FEC84A',NULL,NULL,1,1,472428,6),(72,'icon3:ppm',1,'Node:icon3',NULL,NULL,1,0,NULL,6),(5,'Demo_Energia:PF1',1,'Node:Demo_Energia',NULL,NULL,1,1,472532,6),(6,'Demo_Energia:A1',1,'Node:Demo_Energia',NULL,NULL,1,1,472528,6),(7,'Demo_Energia:V1',1,'Node:Demo_Energia',NULL,NULL,1,1,472524,6),(8,'Demo_Energia:P_TOT',1,'Node:Demo_Energia',NULL,NULL,1,1,472520,6),(9,'Demo_Energia:KWH_TOT',1,'Node:Demo_Energia',NULL,NULL,2,1,117,2),(10,'mini Pro 1:Temperatura',1,'Node:000018FE34FEC84A',NULL,NULL,1,1,472456,6),(12,'mini Pro 1:ppm',1,'Node:000018FE34FEC84A',NULL,NULL,1,1,472424,6),(13,'mini Pro 2:Temperatura',1,'Node:000018FE34FE3BDD',NULL,NULL,1,1,449400,6),(19,'mini Pro 2:ppm',1,'Node:000018FE34FE3BDD',NULL,NULL,1,1,410700,6),(27,'Mini 1 temp',1,'Node:000018FE34A1433A',NULL,NULL,1,1,70996,6),(38,'DZ Rpi Temp',1,'Node:Diszasa_Rpi',NULL,NULL,1,1,2216,6),(22,'CM Diszada Camara Fria',1,'Diszasa_DEMO',NULL,NULL,1,1,404352,6),(29,'mini 1 ppm',1,'Node:000018FE34A1433A',NULL,NULL,1,1,70980,6),(28,'mini 1 hum',1,'Node:000018FE34A1433A',NULL,NULL,1,1,70992,6),(56,'-15',1,'Node:000018FE34FE3BDD',NULL,NULL,1,1,1272,6),(66,'max temp Rpi',1,'Node:Diszasa_Rpi',NULL,NULL,2,0,NULL,2),(70,'icon3:Temperatura',1,'Node:icon3',NULL,NULL,1,0,NULL,6),(71,'icon3:Humidity',1,'Node:icon3',NULL,NULL,1,0,NULL,6),(57,'15',1,'Node:000018FE34FE3BDD',NULL,NULL,1,1,1256,6),(58,'1500 ppm',1,'Node:000018FE34FE3BDD',NULL,NULL,1,1,1108,6),(68,'min temp Rpi 2',1,'Node:Diszasa_Rpi',NULL,NULL,2,0,NULL,2);
/*!40000 ALTER TABLE `feeds` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `input`
--

DROP TABLE IF EXISTS `input`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `input` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT NULL,
  `name` text,
  `description` text,
  `nodeid` text,
  `processList` text,
  `time` datetime DEFAULT NULL,
  `value` float DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=29 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `input`
--

LOCK TABLES `input` WRITE;
/*!40000 ALTER TABLE `input` DISABLE KEYS */;
INSERT INTO `input` VALUES (26,1,'Humidity','','icon3','1:71',NULL,NULL),(28,1,'ppm','','icon3','1:72',NULL,NULL),(4,1,'KWH_TOT','','Demo_Energia','23:9',NULL,NULL),(5,1,'P_TOT','','Demo_Energia','1:8',NULL,NULL),(6,1,'V1','','Demo_Energia','1:7',NULL,NULL),(7,1,'A1','','Demo_Energia','1:6',NULL,NULL),(8,1,'PF1','','Demo_Energia','1:5',NULL,NULL),(9,1,'Temperatura','','000018FE34FEC84A','1:10',NULL,NULL),(10,1,'Humedad','','000018FE34FEC84A','1:11',NULL,NULL),(11,1,'ppm','','000018FE34FEC84A','1:12',NULL,NULL),(25,1,'Temperatura','','icon3','1:70',NULL,NULL),(12,1,'Temperatura','','000018FE34FE3BDD','1:13,2:1.0E-6,3:-15,1:56,2:1.0E-6,3:15,1:57',NULL,NULL),(18,1,'ppm','','000018FE34FE3BDD','1:19,2:1.0E-6,3:1500,1:58',NULL,NULL),(22,1,'Humedad','','000018FE34A1433A','1:28',NULL,NULL),(19,1,'Temperatura','','000018FE34FECD96','1:22',NULL,NULL),(21,1,'Temperatura','','000018FE34A1433A','1:27',NULL,NULL),(23,1,'ppm','','000018FE34A1433A','1:29',NULL,NULL),(24,1,'temperatura','','Diszasa_Rpi','1:38,27:66,1:67,28:68',NULL,NULL);
/*!40000 ALTER TABLE `input` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `multigraph`
--

DROP TABLE IF EXISTS `multigraph`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `multigraph` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text,
  `userid` int(11) DEFAULT NULL,
  `feedlist` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=14 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `multigraph`
--

LOCK TABLES `multigraph` WRITE;
/*!40000 ALTER TABLE `multigraph` DISABLE KEYS */;
INSERT INTO `multigraph` VALUES (2,'Voltaje',1,'[{\"id\":\"7\",\"name\":\"Demo_Energia:V1\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff8b00\",\"timeWindow\":86400000}]'),(3,'Amperios',1,'[{\"id\":\"6\",\"name\":\"Demo_Energia:A1\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff3815\",\"timeWindow\":86400000}]'),(4,'PF1 vrs P_TOT',1,'[{\"id\":\"8\",\"name\":\"Demo_Energia:P_TOT\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"004000\",\"timeWindow\":86400000},{\"id\":\"5\",\"name\":\"Demo_Energia:PF1\",\"datatype\":\"1\",\"left\":false,\"right\":true,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"f99500\"}]'),(5,'P_TOT vrs Temp',1,'[{\"id\":\"13\",\"name\":\"mini Pro 2:Temperatura\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"143dfc\",\"timeWindow\":86400000},{\"id\":\"8\",\"name\":\"Demo_Energia:P_TOT\",\"datatype\":\"1\",\"left\":false,\"right\":true,\"fill\":true,\"end\":0,\"skipmissing\":true,\"lineColour\":\"21a600\"}]'),(6,'Temp 1 Demo',1,'[{\"id\":\"2\",\"name\":\"icon3:Temperatura\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"012e89\",\"timeWindow\":86400000},{\"id\":\"10\",\"name\":\"mini Pro 1:Temperatura\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"33fbf8\"},{\"id\":\"13\",\"name\":\"mini Pro 2:Temperatura\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":true,\"end\":0,\"skipmissing\":true,\"lineColour\":\"00fbcd\"},{\"id\":\"27\",\"name\":\"Mini 1 temp\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"0000ff\"},{\"id\":\"57\",\"name\":\"15\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff0000\"}]'),(7,'Humedad',1,'[{\"id\":\"3\",\"name\":\"icon3:Humidity\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"timeWindow\":86400000,\"lineColour\":\"80ffff\"},{\"id\":\"11\",\"name\":\"mini Pro 1:Humedad\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"00a2a2\"},{\"id\":\"28\",\"name\":\"mini 1 hum\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"00a222\"}]'),(8,'PPM',1,'[{\"id\":\"12\",\"name\":\"mini Pro 1:ppm\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"timeWindow\":86400000,\"lineColour\":\"141414\"},{\"id\":\"19\",\"name\":\"mini Pro 2:ppm\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"660066\"},{\"id\":\"18\",\"name\":\"icon3:ppm\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"6e6e6e\"},{\"id\":\"29\",\"name\":\"mini 1 ppm\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true},{\"id\":\"58\",\"name\":\"1500 ppm\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff0000\"}]'),(10,'Diszada pre-camara',1,'[{\"id\":\"22\",\"name\":\"CM Diszada Camara Fria\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"timeWindow\":10800000,\"lineColour\":\"2d22f2\"},{\"id\":\"56\",\"name\":\"-15\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff0000\"},{\"id\":\"57\",\"name\":\"15\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff0000\"}]'),(11,'Diszasa Temp 2',1,'[{\"id\":\"38\",\"name\":\"DZ Rpi Temp\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"timeWindow\":604800000},{\"id\":\"22\",\"name\":\"CM Diszada Camara Fria\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true},{\"id\":\"56\",\"name\":\"-15\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff0000\"},{\"id\":\"57\",\"name\":\"15\",\"datatype\":\"1\",\"left\":true,\"right\":false,\"fill\":false,\"end\":0,\"skipmissing\":true,\"lineColour\":\"ff0000\"}]');
/*!40000 ALTER TABLE `multigraph` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `notify`
--

DROP TABLE IF EXISTS `notify`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `notify` (
  `userid` int(11) NOT NULL,
  `email` varchar(30) DEFAULT NULL,
  `enabled` int(11) NOT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `notify`
--

LOCK TABLES `notify` WRITE;
/*!40000 ALTER TABLE `notify` DISABLE KEYS */;
INSERT INTO `notify` VALUES (1,'luiscd7@gmail.com',1);
/*!40000 ALTER TABLE `notify` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `openbem_projects_v3`
--

DROP TABLE IF EXISTS `openbem_projects_v3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `openbem_projects_v3` (
  `project_id` int(11) NOT NULL AUTO_INCREMENT,
  `project_name` varchar(30) DEFAULT NULL,
  `project_description` text,
  `project_owner` int(11) DEFAULT NULL,
  `project_collaborators` text,
  `project_mdate` int(11) DEFAULT NULL,
  PRIMARY KEY (`project_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `openbem_projects_v3`
--

LOCK TABLES `openbem_projects_v3` WRITE;
/*!40000 ALTER TABLE `openbem_projects_v3` DISABLE KEYS */;
/*!40000 ALTER TABLE `openbem_projects_v3` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `openbem_scenarios_v3`
--

DROP TABLE IF EXISTS `openbem_scenarios_v3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `openbem_scenarios_v3` (
  `scenario_id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) DEFAULT NULL,
  `scenario_meta` text,
  `scenario_data` text,
  PRIMARY KEY (`scenario_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `openbem_scenarios_v3`
--

LOCK TABLES `openbem_scenarios_v3` WRITE;
/*!40000 ALTER TABLE `openbem_scenarios_v3` DISABLE KEYS */;
/*!40000 ALTER TABLE `openbem_scenarios_v3` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rememberme`
--

DROP TABLE IF EXISTS `rememberme`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rememberme` (
  `userid` int(11) DEFAULT NULL,
  `token` varchar(40) DEFAULT NULL,
  `persistentToken` varchar(40) DEFAULT NULL,
  `expire` datetime DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rememberme`
--

LOCK TABLES `rememberme` WRITE;
/*!40000 ALTER TABLE `rememberme` DISABLE KEYS */;
INSERT INTO `rememberme` VALUES (1,'c720dd43bf2577a77621446d30edf1e4272d3419','fde95ee1f467060d1a866c1351e01afd5e633156','2015-09-13 05:27:00'),(1,'3af62e281ec9913842e252626645b08a714400e4','049f82480d3bb3f46075698f3e79fb57ebcc58f4','2015-09-13 10:55:22');
/*!40000 ALTER TABLE `rememberme` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `report`
--

DROP TABLE IF EXISTS `report`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `report` (
  `userid` int(11) NOT NULL,
  `settings` text,
  PRIMARY KEY (`userid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `report`
--

LOCK TABLES `report` WRITE;
/*!40000 ALTER TABLE `report` DISABLE KEYS */;
INSERT INTO `report` VALUES (1,'{\"histogramFeed\":0,\"thresholdA\":1000,\"thresholdB\":3000,\"defaultPeriod\":30,\"defaultMode\":\"energy\",\"unitprice\":0.16,\"fixedrate\":0}');
/*!40000 ALTER TABLE `report` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) DEFAULT NULL,
  `email` varchar(30) DEFAULT NULL,
  `password` varchar(64) DEFAULT NULL,
  `salt` varchar(32) DEFAULT NULL,
  `apikey_write` varchar(64) DEFAULT NULL,
  `apikey_read` varchar(64) DEFAULT NULL,
  `lastlogin` datetime DEFAULT NULL,
  `admin` int(11) NOT NULL,
  `gravatar` varchar(30) DEFAULT NULL,
  `name` varchar(30) DEFAULT NULL,
  `location` varchar(30) DEFAULT NULL,
  `timezone` varchar(64) DEFAULT 'UTC',
  `language` varchar(5) DEFAULT 'en_EN',
  `bio` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'luiscd7','luiscd7@gmail.com','9228f05d6508ca74c658e561d103273930898c4f5b48aff9130b2cec68f07132','35e','d5c746f11b3d2b34ba192ab8f22efe87','d49c167eedd88bd8b611ef6e5a817fbd',NULL,1,'','Luis Carlos Duarte','El Salvador','America/El_Salvador','es_ES','www.solucionesempresariales.us');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'sees'
--

--
-- Dumping routines for database 'sees'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-09-06 13:04:22
