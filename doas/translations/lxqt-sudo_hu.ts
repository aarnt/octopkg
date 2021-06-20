<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="hu">
<context>
    <name>PasswordDialog</name>
    <message>
        <location filename="../passworddialog.ui" line="6"/>
        <source>LXQt sudo</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="42"/>
        <source>Copy command to clipboard</source>
        <translation>Parancs másolása vágólapra</translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="45"/>
        <source>&amp;Copy</source>
        <translation>&amp;Másolás</translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="83"/>
        <source>The requested action needs administrative privileges.&lt;br&gt;Please enter your password.</source>
        <translation>A kért művelethez adminisztrátori jogosultság szükséges. &lt;br&gt;Írja be a jelszavát.</translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="106"/>
        <source>LXQt sudo backend</source>
        <translation>LXQt sudo backend</translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="109"/>
        <source>A program LXQt sudo calls in background to elevate privileges.</source>
        <translation>Az LXQt sudo program a háttérben megszerzi a jogosultságokat.</translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="119"/>
        <source>Command:</source>
        <translation>Parancs:</translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="126"/>
        <source>Password:</source>
        <translation>Jelszó:</translation>
    </message>
    <message>
        <location filename="../passworddialog.ui" line="133"/>
        <source>Enter password</source>
        <translation>Adjon meg jelszót</translation>
    </message>
    <message>
        <location filename="../passworddialog.cpp" line="60"/>
        <source>Attempt #%1</source>
        <translatorcomment>Próba #%1</translatorcomment>
        <translation>%1. próbálkozás</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../sudo.cpp" line="75"/>
        <source>Usage: %1 option [command [arguments...]]

GUI frontend for %2/%3

Arguments:
  option:
    -h|--help      Print this help.
    -v|--version   Print version information.
    -s|--su        Use %3(1) as backend.
    -d|--sudo      Use %2(8) as backend.
  command          Command to run.
  arguments        Optional arguments for command.

</source>
        <translation>Használat: %1 opció [parancs [paraméterek...]]

A %2/%3 grafikus felhasználói felülete

paraméterek:
  Opciók:
    -h|--help      Kiírja ezt a szöveget..
    -v|--version   Kiírja a verziószámot..
    -s|--su        %3(1) használata backendként.
    -d|--sudo      %2(8) használata backendként.
  parancs           A végrehajtandó parancs.
  argumentumok        Választható argumentumok a parancshoz.

</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="92"/>
        <source>%1 version %2
</source>
        <translation>%1 verzió %2
</translation>
    </message>
</context>
<context>
    <name>Sudo</name>
    <message>
        <location filename="../sudo.cpp" line="195"/>
        <source>%1: no command to run provided!</source>
        <translation>%1: nincs végrehajtandó parancs!</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="202"/>
        <source>%1: no backend chosen!</source>
        <translation>%1: nincs backend kiválasztva!</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="219"/>
        <source>Syscall error, failed to fork: %1</source>
        <translation>Rendszerhívás hiba, nem sikerült forkot létrehozni: %1</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="246"/>
        <source>unset</source>
        <extracomment>shouldn&apos;t be actually used but keep as short as possible in translations just in case.</extracomment>
        <translation type="unfinished">nincs</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="295"/>
        <source>%1: Detected attempt to inject privileged command via LC_ALL env(%2). Exiting!
</source>
        <translation>%1: Észlelt próbálkozás, hogy egy kiemelt parancsot próbált futtatni ezzel: LC_ALL env(%2). Bezárás!
</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="337"/>
        <source>Syscall error, failed to bring pty to non-block mode: %1</source>
        <translation type="unfinished">Rendszerhívás hiba, nem sikerült megnyitni a pszeudoterminált non-block módban: %1</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="345"/>
        <source>Syscall error, failed to fdopen pty: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="314"/>
        <source>%1: Failed to exec &apos;%2&apos;: %3
</source>
        <translation>%1: A(z) &apos;%2&apos;:%3 végrehajtása sikertelen
</translation>
    </message>
    <message>
        <location filename="../sudo.cpp" line="376"/>
        <source>Child &apos;%1&apos; process failed!
%2</source>
        <translation>A(z) &apos;%1&apos; gyermekfolyamat végrehajtása nem sikerült!
%2</translation>
    </message>
</context>
</TS>
