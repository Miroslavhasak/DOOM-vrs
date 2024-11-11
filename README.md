# DOOM-vrs
Semestralne zadanie; autori: Martin Mizner, Miroslav Hasák, Matej Havetta

# Predstava
Chceme rozbehnúť na mikrokontroléri veľmi jednoduchú verziu tento hry aby sme dokázali naše schopnosti naprogramovať program ktorý komunikuje s displejom, s tlačítkami, predviesť poznatky z 3D grafiky, vytvoriť jednoduché používateľské rozhranie.

#Naše plány:
Názov projektu: DOOM

Fyzická predstava: 
Keď spustíme v počítači "run", tak sa ukáže Intro obrazovka s Main menu
MAIN MENU: 
1. krátky návod, môže sa dať otvoriť cez tlačidlo výberu, v našom prípade stačí, keď kvôli jednoduchosti
zobrazíme návod priamo v Intro obrazovke
2. tlačidlo play - asi jediná vec s ktorou bude možné interagovať, zatiaľ netreba robiť menu, kde sa dá prepínať
medzi viacerými možnosťami.


HRA:
kvôli jednoduchosti bude herný priestor štvorcová aréna, neskôr môžeme pridať steny a múry aby to bolo ako labyrint
kde nevieš čo je za rohom
Hra sa bude riadiť 6 tlačidlami:
1. dopredu
2. dozadu
3. doprava
4. doľava
5. strieľať
6. ukončiť hru / pause

- Mechanika streľby: predpokladajme, že strela okamžite zasiahne. po vystrelení sa pomocou cyklu
prehľadá priestor v smere strely a ak zasiahne nepriateľa, tak ho 1 hit zabije (jednoduchšie).
treba ošetriť, aby sme nestrieľali viac, než treba - ak bude mať hra 10 fps, tak viac, než 10 
striel za sekundu nebude strieľať. Celkovo by bolo dobré hru taktovať na ticky.
- Mechanika pohybu: hráč bude mať svoju X a Z polohu, a uhol otočenia Phi.

Nepriatelia (démoni/sprites) nemusia mať zložitú logiku, stačí, že pôjdu priamo do teba a v určitej vzdialenosti
budú mať melee damage. Bude treba naprogramovať level, kde budú od začiatku prítomní a pozabíjaním všetkých spritov hra skončí.
Demonov bude reprezentovať obdĺžnik

Hra sa teda končí keď pozabíjame všetkých spritov
ukáže sa nejaké skóre na 3s, potom sa vrátime do MAIN MENU
Skóre: môžeme hodnotiť čas, za ktorý sme zvládli level, keďže počet nepriateľov bude asi zatiaľ rovnaký
Ak hráč zomrie, tak sa cyklus hry ukončí a ukáže sa Game over obrazovka na 1s, potom sa dostaneme do MAIN MENU


GRAFIKA:
Predpokladám, že bude k dispozícii displej, kde bude možné vykreslovať základné útvaru ako štvorec, kruh, mnohouholník
DOOM nie je úplne 3D hra, lebo sa nedá pozrieť nad seba ani pod seba, len doprava a doľava. Sú jednoduché vzorce
ako vypočítať súradnice bodov na 2D obrazovke podľa toho ako sú ďaleko a ako ste voči ním otočení.
Neviem koľko mikrokontrolér utiahne, ale podľa mňa 5 až 10 fps by mal zvládnuť, hlavne ak zobrazujeme zopár
pohybujúcich sa štvorcov.
Pokiaľ ide o zobrazovanie menu, bude to len text a nejaké útvary, ktorým dáme súradnice.

HARDVÉR:
Potrebujeme displej a tlačítka.

DELBA PRÁCE
ako vidno, projekt možno rozdeliť do týchto oblastí:
1. komunikácia pc s obrazovkou, posiela spracovaný obraz, nakonfigurovať zbernicu
2. kód na vykresľovanie obrazu, 3D projekcia a zobrazovanie textu
3. komunikácia plc s tlačidlami, sleduje output, nakonfigurovať zbernicu
4. mechanika MAIN MENU, asi cyklus, zobrazuje menu a čaká na input z tlačidiel
5. mechanika "in game" MENU, podobné MAIN MENU
6. mechanika samotnej hry, pohyb hráča, pohyb spritov, skóre
Tento zoznam sa ešte rozrastie lebo nevieme do čoho ideme

Pre 3 ľudí by sa to optimálne delilo
1. osoba: 1 2 (grafika a komunikácia s obrazovkou)
2. osoba: 3 6 (tlačidlá a mechanika hry)
3. osoba: 4 5 (Mechanika MENU)
