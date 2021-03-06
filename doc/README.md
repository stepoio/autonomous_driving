### Update 28.03.2018
Bitte keine eigenmächtigen Änderungen (sprich: Packages entfernen) an der Präambel vornehmen. 

### Update 27.03.2018
Gab einige Probleme mit der Bibliographie (Kompilierung, Zeilenumbrüche, deutsche Sprache und was weiß ich), wenn ihr noch Seltsames entdeckt bitte Issue eröffnen, aber ich hoffe ich habe alles gesehen.
Momentaner Style ist IEEE, Bibliographie-Programm ist biber (biblatex macht komische Sachen, bitte nicht eigenmächtig ändern sondern gemeinsam nach ner Lösung suchen falls das zu einem Probkem wird).
Ihr müsst für euren Latex-Editor einrichten, dass ihr das tex-File quasi 3-mal erzeugen müsst: latex - biber - nochmal latex, siehe: https://tex.stackexchange.com/questions/13509/biblatex-in-a-nutshell-for-beginners

Hier ein Guide für verschiedene Editor: 
https://tex.stackexchange.com/questions/154751/biblatex-with-biber-configuring-my-editor-to-avoid-undefined-citations

Sonst kann es zu Problemen beim Erzeugen des Dokuments kommen! 

### Update 07.03.18
Zunächst: Alles steht zur Debatte, wenn etwas unsinnig erscheint sollten wir das zeitnah(!) diskutieren. Bitte nicht erst im April ;)

Im Dokument 'projektdoku.tex' findet Ihr Vorlagen um Eure Nodes/Nodeletes kurz (!) zu beschreiben. Erklärt nur die grundlegenden Funktionen und ggf. Fallstricke. Yannick hat sich zwar bereit erklärt, das zu übernehmen, aber es schadet nicht wenn der geistige Schöpfer sich auch damit beschäftigt.
Wenn Ihr andere Nodes referenziert, nutzt die Labels, die ich bereits vorgegeben habe. Der erste Node ist ein Dummy, da könnt Ihr abgucken was inhaltlich in etwa gefordert ist und wie man referenziert.

Der Quellcode selbst wird im Anhang zu finden sein. Derjenige, der einen Node dokumentiert, sollte sich (ggf. zu einem späteren Zeitpunkt, wenn der Code komplett ist) auch darum kümmern, dass der Quellcode im Anhang eingebunden wird.
Zu diesem Zweck habe ich in der Präambel des tex-Files eine globale Formatierungen vorgegebn und Euch zwei Möglichkeiten anhand von Beispielen im Anhang angegeben, wie man Code einbindet (inline oder als Import aus einer Quellcode-Datei).
Beide Versionen funktionieren, hauptsache Ihr nutzt [style=customcpp]; falls Änderungswünsche bestehen müssen wir dann nämlich nur in der Präambel was anpassen. 

Was noch etwas in der Schwebe ist: zur Erläuterung der Nodes würden sich Diagramme anbieten. Diese werden vmtl. nachträglich eingefügt und sind erstmal Aufgabe von Yannick und mir. 

Aussichten: die anderen Parts der Doku wollen auch liebevolle Pflege erfahren. Wir werden in Kürze vmtl. per Slack oder physisches Treffen entscheiden, wer welchen Teil am besten betreuen kann. Oder der Würfel entscheidet. 

### Ankündigung
Sobald die Klausurenphase vorbei ist (ca. Anfang März), werdet Ihr von mir Zuweisungen bekommen, welchen Teil der Doku ihr betreuen sollt. 
Bei Fragen zu LaTex, Form, Vorlagen, Beispiele etc. könnt Ihr mich gerne anschreiben.

Anne

### Zweck
Dieser Ordner soll enthalten:
* Fotos (ja, Fotos. Perfekt für gitlab)
* Anhänge, z.B. Spezifikationen, Anleitungen
* Bibliographie
* tex-Files, die die Doku betreffen

### Wichtig
Bitte gebt alle Quellen in der Bibliographie an, zitiert korrekt (Plagiat vermeiden) und achtet unbedingt auf das Urheberrecht von Grafiken, Bildern etc.
Die Doku wird im Internet vmtl. frei zugänglich sein. 