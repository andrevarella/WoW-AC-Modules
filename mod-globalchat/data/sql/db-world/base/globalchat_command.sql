DELETE FROM `command` WHERE `name` IN ('global','g','chat','c','joinglobal','leaveglobal','genable','gdisable','gmute','gunmute','ginfo','galliance','ghorde','gblacklist','gblacklist add','gblacklist remove','gblacklist reload');
INSERT INTO `command` (`name`, `security`, `help`) VALUES
('global', 0, 'Comando: .global $texto\nEscreve uma mensagem no GlobalChat.\nComandos adicionais: .joinglobal & .leaveglobal'),
('g', 0, 'Comando: .g $texto\nEscreve uma mensagem no GlobalChat.\nComandos adicionais: .joinglobal & .leaveglobal'),
('chat', 0, 'Comando: .chat $texto\nEscreve uma mensagem no GlobalChat.\nComandos adicionais: .joinglobal & .leaveglobal'),
('c', 0, 'Comando: .c $texto\nEscreve uma mensagem no GlobalChat.\nComandos adicionais: .joinglobal & .leaveglobal'),
('joinglobal', 0, 'Join the GlobalChat.'),
('leaveglobal', 0, 'Leave the GlobalChat.'),
('ginfo', 1, 'Syntax: $playername\nGives GlobalChat information about a player.'),
('genable', 1, 'Enables GlobalChat.'),
('gdisable', 1, 'Disables GlobalChat.'),
('gmute', 1, 'Syntax: $playername $bantime $reason\nMutes a player in the GlobalChat\n$bantime: Negative values lead to perment mute. Otherwise use a timestring like "1d2h30s".'),
('gunmute', 1, 'Syntax: $playername\nUnmutes player in the GlobalChat.'),
('galliance', 1, 'Syntax: .galliance $text\nWrite a message in the Alliance GlobalChat.'),
('ghorde', 1, 'Syntax: .ghorde $text\nWrite a message in the Horde GlobalChat.'),
('gblacklist', 1, 'Type .gblacklist to see the list of possible subcommands or .help gblacklist $subcommand to see info on subcommands.'),
('gblacklist add', 1, 'Syntax: .gblacklist add $text\nAdds a phrase to the GlobalChat Profanity Blacklist.'),
('gblacklist remove', 1, 'Syntax: .gblacklist remove $text\nRemoves a phrase from the GlobalChat Profanity Blacklist.'),
('gblacklist reload', 1, 'Reloads the GlobalChat Profanity Blacklist.');


-- players:
-- .joinglobal / .leaveglobal = sai do chat

-- .global / .g = escreve msg no GlobalChat.
-- .chat   / .c = escreve msg no GlobalChat.




-- GM only:
-- .genable / .gdisable = o GM desativa o chat para todo mundo (aparace no chat quando o global foi ativado ou desativado + nick do GM)
-- .ginfo + nome do player (infos: in chat, last message, muted, mute time, total mutes, banned)
-- .gmute + nome do player = mute no player

-- .galliance = escreve msg no GlobalChat p Alliance (p/ GM only) - caso GlobalChat.FactionSpecific = 1, ao digitar .g só manda para membros da mesma facção caso for player
-- .ghorde    = escreve msg no GlobalChat p Horde    (p/ GM only)

-- .gblacklist add + Palavra (os players conseguem mandar msg mas a palavra blacklistada aparece como *****)
-- .gblacklist remove + palavra
-- .gblacklist reload Reloads the GlobalChat Profanity Blacklist.