# TheEscapists

The Escapists este un joc de strategie în care joci rolul unui deținut și scopul tău este să evadezi din diverse închisori.

## Gameplay-ul presupune următoarele elemente principale:

* **Respectarea programului închisorii:** prezența de dimineață, mesele, jobul - pentru a nu stârni suspiciunea gardienilor.
* **Crafting:** Furi sau faci schimb de obiecte cu alți deținuți. Aceste obiecte de bază le combini pentru a crea unelte necesare evadării.
* **Planificarea:** Gândirea și punerea în aplicare a unei strategii de evadare.

## Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o clasă șablon cu sens; minim **2 instanțieri**
  - [ ] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
- [ ] minim 80-90% din codul propriu să fie C++
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.3` sau `v1.0`
- [ ] code review #3 2 proiecte

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC folosind Git Bash:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```

Pentru a configura cu ASan, avem opțiunea `-DUSE_ASAN=ON` (nu merge pe Windows cu GCC):
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON
# sau ./scripts/cmake.sh configure -e "-DUSE_ASAN=ON"
```


La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.


2. Pasul de compilare
```sh
cmake --build build --config Debug --parallel 6
# sau ./scripts/cmake.sh build
```

Cu opțiunea `parallel` specificăm numărul de fișiere compilate în paralel.


3. Pasul de instalare (opțional)
```sh
cmake --install build --config Debug --prefix install_dir
# sau ./scripts/cmake.sh install
```

Vezi și [`scripts/cmake.sh`](scripts/cmake.sh).

Observație: folderele `build/` și `install_dir/` sunt adăugate în fișierul `.gitignore` deoarece
conțin fișiere generate și nu ne ajută să le versionăm.


## Instrucțiuni pentru a rula executabilul

Există mai multe variante:

1. Din directorul de build (implicit `build`). Executabilul se află la locația `./build/oop` după ce a fost rulat pasul de compilare al proiectului (`./scripts/cmake.sh build` - pasul 2 de mai sus).

```sh
./build/oop
```

2. Din directorul `install_dir`. Executabilul se află la locația `./install_dir/bin/oop` după ce a fost rulat pasul de instalare (`./scripts/cmake.sh install` - pasul 3 de mai sus).

```sh
./install_dir/bin/oop
```

3. Rularea programului folosind Valgrind se poate face executând script-ul `./scripts/run_valgrind.sh` din rădăcina proiectului. Pe Windows acest script se poate rula folosind WSL (Windows Subsystem for Linux). Valgrind se poate rula în modul interactiv folosind: `RUN_INTERACTIVE=true ./scripts/run_valgrind.sh`

Implicit, nu se rulează interactiv, iar datele pentru `std::cin` sunt preluate din fișierul `tastatura.txt`.

```sh
RUN_INTERACTIVE=true ./scripts/run_valgrind.sh
# sau
./scripts/run_valgrind.sh
```

4. Pentru a rula executabilul folosind ASan, este nevoie ca la pasul de configurare (vezi mai sus) să fie activat acest sanitizer. Ar trebui să meargă pe macOS și Linux. Pentru Windows, ar merge doar cu MSVC (nerecomandat).

Comanda este aceeași ca la pasul 1 sau 2. Nu merge combinat cu Valgrind.

```sh
./build/oop
# sau
./install_dir/bin/oop
```

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).

## Resurse

https://github.com/nlohmann/json

https://www.sfml-dev.org/documentation/3.1.0/

https://en.wikipedia.org/wiki/A*_search_algorithm

https://www.geeksforgeeks.org/dsa/a-search-algorithm/

https://www.spriters-resource.com/pc_computer/theescapists/

https://www.mapeditor.org/
