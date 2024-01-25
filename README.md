# Conway's Game of Life

## Descriere

Conway's Game of Life este un automat celular, un "joc" cu 0 jucatori care se desfasoara pe o grila patrata bidimensionala. Celulele de pe grila pot avea valoarea 1 (vie) sau 0 (moarta) si evolueaza conform urmatoarelor reguli:

- o celula vie cu mai putin de doi vecini vii moare.
- o celula vie cu mai mult de trei vecini vii moare.
- o celula vie cu doi sau trei vecini vii supravietuieste, nemodificata, in generatia urmatoare.
- o celula moarta cu exact trei vecini vii devine vie.

Configuratia initiala a celulelor poate fi creata de un om, dar toate generatiile ulterioare sunt complet determinate de regulile de mai sus. Scopul jocului este sa gasesti modele care evolueaza in moduri interesante.

## Echipa proiectului:
- Calugaritoiu Ion-Victor 342C1
- Rizea Adelina-Maria 342C1
- Popescu Ioana-Denisa 342C1

## Numele asistentului:
Ouatu Andrei-Catalin

## Usage
Pentru a genera o tabla initiala: <br>
    python3 ./in/gen_test.py [width] [height]

Pentru compilare & rulare: <br>
    make <br>
    make run_large (500x500 test)<br>
    OR <br>
    make run_small (10x10 test)

Pentru testare timpi rulare + corectitudine algoritmi:<br>
    ./test.sh [small/large]

