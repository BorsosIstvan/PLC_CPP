# Mijn Simpele PLC + Modbus Project

Dit project simuleert een kleine PLC met een TON-timer en Modbus RTU ondersteuning. Je kunt het gebruiken om te leren hoe je een HMI koppelt aan een PLC-achtig systeem.

## 🧩 Functionaliteiten

- TON (Timer-On Delay) zoals in echte PLC's
- Flip-flop logica
- Modbus RTU communicatie
- Aansturing van coils (QX) en registers (QW)
- Seriële communicatie via COM-poort

## 🛠️ Benodigdheden

- C++ compiler (g++)
- Windows of Linux
- [VS Code](https://code.visualstudio.com/) aanbevolen
- Seriële poort voor communicatie met een HMI (optioneel)
- HMI software die Modbus RTU ondersteunt

## 🗂️ Projectstructuur

## 🚀 Compileren en starten

### Gebruik VS Code:
Er is een `tasks.json` aanwezig onder `.vscode/`.

Druk op `Ctrl+Shift+B` → *"Build en Run"*

### Of gebruik Makefile:

make

Gebruik:

make → compileert

make run → compileert en draait

make clean → verwijdert build/
De executable staat dan in build/programma.exe.

## 📡 Modbus instellingen
Baudrate: 9600

Poort: COM6 (pas aan in setup())

Slave ID: 1

Gebruik bijvoorbeeld een HMI zoals ModbusPoll of Weintek EasyBuilder

## 📜 Licentie
MIT-licentie – Vrij te gebruiken en aan te passen.