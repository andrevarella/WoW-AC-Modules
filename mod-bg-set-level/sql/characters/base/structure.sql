-- Dumping structure for table acore_characters.custom_bg_level
CREATE TABLE IF NOT EXISTS `custom_bg_level` (
                                                 `PlayerID` int(11) NOT NULL,
                                                 `original_level` int(11) NOT NULL,
                                                 `original_xp` int(11) NOT NULL,
                                                 PRIMARY KEY (`PlayerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table acore_characters.custom_bg_level: ~0 rows (approximately)
/*!40000 ALTER TABLE `custom_bg_level` DISABLE KEYS */;
/*!40000 ALTER TABLE `custom_bg_level` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
