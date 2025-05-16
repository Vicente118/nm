# La Commande Unix `nm` : Fonctionnement et Concepts

## Introduction

La commande `nm` est un utilitaire en ligne de commande Unix qui permet d'afficher les symboles contenus dans des fichiers objets, bibliothèques statiques et exécutables. Son nom vient de "name list" (liste de noms). C'est un outil essentiel pour l'analyse et le débogage de programmes, particulièrement utile pour les développeurs systèmes et les programmeurs C/C++.

## Fonctionnement de la commande `nm`

### Syntaxe de base

```bash
nm [options] [fichiers]
```

### Exemples d'utilisation

```bash
nm executable                  # Affiche les symboles d'un exécutable
nm -D libexample.so           # Affiche les symboles dynamiques d'une bibliothèque partagée
nm -A *.o                     # Affiche les symboles de tous les fichiers objets avec leur nom
nm --demangle executable      # Affiche les noms de symboles C++ démanglés
```

### Options principales

- `-A` / `--print-file-name` : Affiche le nom du fichier avant chaque symbole
- `-C` / `--demangle` : Démangle les noms de symboles C++
- `-D` / `--dynamic` : Affiche les symboles dynamiques
- `-u` / `--undefined-only` : Affiche uniquement les symboles non définis
- `-g` / `--extern-only` : Affiche uniquement les symboles externes
- `-a` / `--debug-syms` : Affiche les symboles de débogage
- `-n` / `--numeric-sort` : Tri des symboles par adresse

### Format de sortie

Pour chaque symbole, `nm` affiche généralement trois colonnes :
1. L'adresse du symbole (en hexadécimal)
2. Le type du symbole (une lettre)
3. Le nom du symbole

Exemple :
```
0000000000001160 T main
0000000000004018 B global_var
                 U printf
```

## Concepts liés à la commande `nm`

### 1. Format ELF (Executable and Linkable Format)

#### Introduction à ELF

ELF est le format standard de fichier binaire utilisé par Linux et d'autres systèmes Unix modernes. Il s'agit d'un format flexible qui peut représenter :
- Des fichiers objets (`.o`)
- Des exécutables
- Des bibliothèques partagées (`.so`)
- Des core dumps

#### Structure d'un fichier ELF

Un fichier ELF est composé de plusieurs parties :

1. **En-tête ELF (ELF Header)** : Contient des informations générales comme l'architecture cible, le type de fichier, etc.
2. **Table d'en-tête de programme (Program Header Table)** : Décrit les segments utilisés pour l'exécution.
3. **Sections** : Contiennent les données du programme (code, données, symboles, etc.).
4. **Table d'en-tête de section (Section Header Table)** : Décrit les sections du fichier.

Parmi les sections importantes :
- `.text` : Code exécutable
- `.data` : Variables initialisées
- `.bss` : Variables non initialisées
- `.symtab` / `.dynsym` : Tables de symboles
- `.strtab` / `.dynstr` : Tables de chaînes de caractères

### 2. Tables de symboles

#### Qu'est-ce qu'un symbole ?

Un symbole est une référence nommée à une adresse mémoire. Il peut représenter :
- Une fonction
- Une variable globale
- Une constante
- Une section
- Etc.

#### Structure d'une entrée de table de symboles

Chaque entrée dans une table de symboles contient :
- Le nom du symbole (un indice dans une table de chaînes)
- La valeur du symbole (généralement une adresse)
- La taille du symbole
- Le type et les attributs (visibilité, liaisons...)
- La section associée

#### Types de symboles courants affichés par `nm`

- `T`, `t` : Texte (code) - majuscule pour global, minuscule pour local
- `D`, `d` : Données initialisées
- `B`, `b` : Données non initialisées (BSS)
- `U` : Symbole non défini (référencé mais défini dans un autre fichier)
- `W` : Symbole faible (peut être redéfini)
- `A` : Symbole absolu (ne change pas pendant la relocation)
- `C` : Symbole commun (non initialisé, peut apparaître dans plusieurs fichiers)
- `R`, `r` : Données en lecture seule

### 3. Processus de compilation et édition de liens

#### Étapes de compilation

1. **Préprocesseur** : Traitement des directives (#include, #define)
2. **Compilation** : Traduction du code source en code assembleur puis en code objet
3. **Édition de liens** : Résolution des symboles et création d'un exécutable

#### Édition de liens et symboles

- **Édition de liens statique** : Les bibliothèques sont intégrées à l'exécutable
- **Édition de liens dynamique** : Les bibliothèques sont chargées au moment de l'exécution
- **Résolution des symboles** : Le lieur connecte les références de symboles avec leur définition

### 4. Visibilité des symboles et portée

#### Visibilité

- **Symboles globaux** : Accessibles depuis d'autres fichiers
- **Symboles locaux** : Accessibles uniquement dans leur fichier de définition
- **Symboles faibles** : Peuvent être redéfinis

#### Attributs de liaison

- **GLOBAL** : Accessible dans tout le programme
- **LOCAL** : Accessible uniquement dans son unité de compilation
- **WEAK** : Comme GLOBAL mais peut être écrasé

### 5. Relocation et chargement dynamique

#### Relocation

Processus qui ajuste les adresses des symboles pour qu'ils pointent vers les bonnes cibles mémoires lorsque le programme est chargé.

#### Chargement dynamique

- **Bibliothèques dynamiques** : Chargées au démarrage du programme ou à la demande
- **Position Independent Code (PIC)** : Code qui peut s'exécuter à n'importe quelle adresse mémoire

## Plan de résolution pour implémenter `nm`

### 1. Analyse des besoins et spécifications

- Étudier les fonctionnalités et options de la commande `nm` native
- Déterminer quelles options implémenter en priorité
- Définir le format de sortie

### 2. Conception globale

- Structure de données pour stocker les symboles
- Architecture modulaire (séparation entre parsing ELF, analyse des symboles et affichage)
- Gestion des erreurs et cas particuliers

### 3. Implémentation du parseur ELF

- Lecture et validation de l'en-tête ELF
- Identification du type de fichier (objet, exécutable, bibliothèque partagée)
- Extraction de la table d'en-tête des sections
- Localisation des sections `.symtab`, `.dynsym`, `.strtab` et `.dynstr`

### 4. Extraction et analyse des symboles

- Parcours des tables de symboles
- Récupération des noms depuis les tables de chaînes
- Classification des symboles par type
- Détermination de la visibilité et autres attributs

### 5. Implémentation des options de tri et filtrage

- Tri des symboles (par nom, adresse)
- Filtrage selon les options (symboles externes, non définis, etc.)
- Démanglage des noms C++ (optionnel)

### 6. Mise en forme et affichage

- Formatage des adresses en hexadécimal
- Affichage du type de symbole (lettre)
- Gestion des différents formats d'affichage selon les options

### 7. Tests et validation

- Tests unitaires pour chaque composant
- Tests d'intégration avec différents types de fichiers
- Comparaison des résultats avec la commande `nm` native
- Tests de performance et optimisations

### 8. Documentation et finalisation

- Documentation du code
- Manuel d'utilisation
- Gestion des cas particuliers et limites

Ce plan vous fournit une feuille de route claire pour implémenter votre propre version de la commande `nm` en C, en commençant par les fonctionnalités de base et en ajoutant progressivement des options plus avancées.