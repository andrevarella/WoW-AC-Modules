DELETE FROM `command` WHERE `name` IN ('cfbg', 'cfbg race');
INSERT INTO `command` (`name`, `security`, `help`) VALUES
('cfbg', 0, 'Crossfaction battleground module commands.'),
('cfbg race', 0, 'Morphs your character to the selected race once you join a crossfaction battleground.\nThe following races are available:\nIf you are Horde: human, dwarf, gnome, draenei, nightelf.\nIf you are Alliance: orc, undead, bloodelf, troll, tauren.');
