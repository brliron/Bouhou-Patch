********** English *********
Before doing this step, you should execute update_chars_list, update
translations\chars.csv with the content of the generated chars_dump.txt, and
set the [Log] PRINT_ALL_CHARS flag in translations\{game_dir}\config.ini to 1.

Reads the dialogs*.txt files, and replace the unknown characters inside of it
with known characters, using the new chars.csv.
The result is stored in dialogs*_all_chars.txt

********** French **********
Avant d'effecture cette partie, vous devez exécuter update_chars_list, puis
mettre à jour translations\chars.csv avec le contenu du fichier chars_dump.txt
généré, et mettre le flag [Log] PRINT_ALL_CHARS dans
translations\{dossier du jeu}\config.ini à 1.

Lit les fichiers dialogs*.txt, et remplace les caractères inconnus dedans par
des caractères obtenus depuis le nouveau chars.csv.
Le résultat est stocké dans dialogs*_all_chars.txt
