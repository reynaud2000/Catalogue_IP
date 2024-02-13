# Documentation du projet

Ouvrir dans votre dossier ce fichier : ./Code/html/index.html

[Page web de la documentation](./Code/html/index.html)


# Prérequis pour le code 
 

1. Installer les bibliothèques essentielles:

    ```
    sudo apt-get install build-essential

    ```
    
2. Installer GTK:

    ```
        sudo apt-get install libgtk-3-dev
        
    ```

3. Installer Pango et Cairo:

    ```
        sudo apt-get install libpango1.0-dev libcairo2-dev
    ```
4. Installer GDB:

 ```
       sudo apt-get install gdb
 ```
5. Installer Nscurses:

   ```
     sudo apt-get install libncurses5-dev libncursesw5-dev
   ```
6. Installer MySQL pour C :

   ```
     sudo apt-get install libmysqlcppconn-dev
   ```
# Pour exécuter le code

1. A faire dans le dossier "code" :
   
```
gcc -o catalogue_ip src/*.c $(pkg-config --cflags --libs gtk+-3.0) -lm -lsqlite3 -g3 -lncurses -g
```

2. Puis exécuter avec cette commande : 
```
./catalogue_ip
```
# IP C-atalogue

Vous allez créer un utilitaire permettant de cataloguer les adresses IP(v4).
L'utilitaire doit permettre de:

1. Stocker des adresses IP, et les changer au lancement
2. Filtrer les adresses via un masque
3. Afficher une adresse IP sous sa forme binaire, hexa, ou décimale
4. Une validation doit être mise en place

## 1) Persistance

Il est demandé que les IP du catalogue soient stockées par le programme puis chargées lors de son ouverture.
Les addresses Ip doivent rester persister entre les executions du programme.

## 2) Filtrage par addresses mac

Il doit être possible de faire une recherche de la facon suivante :

- on demande à l'utilisateur une addresse
- on demande à l'utilisateur un masque
- on renvoie les addresses stockées qui appartiennent au même sous-réseau que celle donnée par l'utilisateur.

## 3) Binaire/hexadécimal/décimal

Les addresses IP doivent être affichées sous leurs formes binaires, hexadécimales, et décimales.

ex :

```
192.168.1.1
11000000.10101000.00000001.00000001
C0.A8.1.1
```

## 4) Validation

Une validation doit être faite pour que l'on ne puisse pas entrer n'importe quoi comme addresses IP.

```
afs.213.sf.23ew // Invalide
300.192.1.2     // Invalide
-12.23.12.23    // Invalide
172.45.223.1    // invalide
```

## Boucle principale

Le programme doit s'afficher comme un menu et interagir avec l'utilisateur.
Il ne doit pas se fermer sans la demande de celui-ci.

exemple d'interaction :

```
 ::::::::                   ::: ::::::::::: :::     :::        ::::::::   ::::::::  :::    ::: ::::::::::
:+:    :+:                :+: :+:   :+:   :+: :+:   :+:       :+:    :+: :+:    :+: :+:    :+: :+:
+:+                      +:+   +:+  +:+  +:+   +:+  +:+       +:+    +:+ +:+        +:+    +:+ +:+
+#+       +#++:++#++:++ +#++:++#++: +#+ +#++:++#++: +#+       +#+    +:+ :#:        +#+    +:+ +#++:++#
+#+                     +#+     +#+ +#+ +#+     +#+ +#+       +#+    +#+ +#+   +#+# +#+    +#+ +#+
#+#    #+#              #+#     #+# #+# #+#     #+# #+#       #+#    #+# #+#    #+# #+#    #+# #+#
 ########               ###     ### ### ###     ### ########## ########   ########   ########  ##########

 a - Add a new IP address
 l - List IP addresses
 s - Search similar by mask
 d - Delete an IP
 q - quit
 s

Enter address : 192.168.1.1
Enter mask : 192.168.255.255

3 addresses found :
1- 192.168.1.42
2- 192.168.5.76
3- 192.168.5.2

c - cancel

1

192.168.1.42
- 1011110111001001011111000100110
- 5e.e4.be.26



```

Vous pouvez utiliser les librairies de votre choix, **excepté arpa/inet.h qui est interdite.**

Tout ajout ou amélioration de fonctionnalité pourra mener à une valorisation de la note si jugé suffisant.

