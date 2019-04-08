********** English *********
Reads the dialogs*_all_chars.txt files, creates a linked list of characters for every text block,
and pass this linked list to CharBuff::dump.
This lets you re-execute the line splitting algorithm with the characters data gathered in step 2,
and you can also try a different line splitting algorithm.
The result is stored in dialogs*_all_chars_v2.txt (those file names are getting quite big...)

Also, CharBuff is initialized without Log::PRINT_ALL_CHARS and Log::PRINT_TRANSLATION, so the output
have way less texts.

********** French **********
Lit les fichiers dialogs*_all_chars.txt, crée une liste chainée de caractères pour chaque paragraphe,
et passe cette liste chainée à CharBuff::dump.
Ceci permet de de reexécuter l'algorithme de séparation de lignes en ayant plus d'informations sur les caractères,
ou en testant avec un autre algorithme.
Le résultat est stocké dans dialogs*_all_chars_v2.txt (les noms commencent à être bien longs...)

Aussi, CharBuff est initialisé sans Log::PRINT_ALL_CHARS ni Log::PRINT_TRANSLATION, donc la sortie contient
beaucoup moins de textes.
