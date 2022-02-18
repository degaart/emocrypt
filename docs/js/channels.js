const channels = {
    "custom":
        "",
    "navy-seal":
        "What the fuck did you just fucking say about me, you little bitch?\n\nI'll have "+
        "you know I graduated top of my class in the Navy Seals, and I've been involved "+
        "in numerous secret raids on Al-Quaeda, and I have over 300 confirmed kills. I "+
        "am trained in gorilla warfare and I'm the top sniper in the entire US armed "+
        "forces. You are nothing to me but just another target. I will wipe you the fuck "+
        "out with precision the likes of which has never been seen before on this Earth, "+
        "mark my fucking words. You think you can get away with saying that shit to me "+
        "over the Internet? Think again, fucker. As we speak I am contacting my secret "+
        "network of spies across the USA and your IP is being traced right now so you "+
        "better prepare for the storm, maggot. The storm that wipes out the pathetic "+
        "little thing you call your life. You're fucking dead, kid. I can be anywhere, "+
        "anytime, and I can kill you in over seven hundred ways, and that's just with my "+
        "bare hands. Not only am I extensively trained in unarmed combat, but I have "+
        "access to the entire arsenal of the United States Marine Corps and I will use "+
        "it to its full extent to wipe your miserable ass off the face of the continent, "+
        "you little shit. If only you could have known what unholy retribution your "+
        "little \"clever\" comment was about to bring down upon you, maybe you would have "+
        "held your fucking tongue. But you couldn't, you didn't, and now you're paying "+
        "the price, you goddamn idiot. I will shit fury all over you and you will drown "+
        "in it.\n\nYou're fucking dead, kiddo.",
    "gnu-linux": 
        "I'd just like to interject for a moment. What you're refering to as Linux, is in"+
        " fact, GNU/Linux, or as I've recently taken to calling it, GNU plus Linux. Linux"+
        " is not an operating system unto itself, but rather another free component of a "+
        "fully functioning GNU system made useful by the GNU corelibs, shell utilities an"+
        "d vital system components comprising a full OS as defined by POSIX.\n\nMany compute"+
        "r users run a modified version of the GNU system every day, without realizing it"+
        ". Through a peculiar turn of events, the version of GNU which is widely used tod"+
        "ay is often called Linux, and many of its users are not aware that it is basical"+
        "ly the GNU system, developed by the GNU Project.\n\nThere really is a Linux, and th"+
        "ese people are using it, but it is just a part of the system they use. Linux is "+
        "the kernel: the program in the system that allocates the machine's resources to "+
        "the other programs that you run. The kernel is an essential part of an operating"+
        " system, but useless by itself; it can only function in the context of a complet"+
        "e operating system. Linux is normally used in combination with the GNU operating"+
        " system: the whole system is basically GNU with Linux added, or GNU/Linux. All t"+
        "he so-called Linux distributions are really distributions of GNU/Linux!",
    "rick-and-morty":
        "To be fair, you have to have a very high IQ to understand Rick and Morty. "+
        "The humour is extremely subtle, and without a solid grasp of theoretical "+
        "physics most of the jokes will go over a typical viewer's head. There's "+
        "also Rick's nihilistic outlook, which is deftly woven into his characterisation- "+
        "his personal philosophy draws heavily from Narodnaya Volya literature, for "+
        "instance. The fans understand this stuff; they have the intellectual capacity "+
        "to truly appreciate the depths of these jokes, to realise that they're not just "+
        "funny- they say something deep about LIFE. As a consequence people who dislike "+
        "Rick & Morty truly ARE idiots- of course they wouldn't appreciate, for instance, "+
        "the humour in Rick's existential catchphrase \"Wubba Lubba Dub Dub,\" which itself "+
        "is a cryptic reference to Turgenev's Russian epic Fathers and Sons. I'm smirking "+
        "right now just imagining one of those addlepated simpletons scratching their "+
        "heads in confusion as Dan Harmon's genius wit unfolds itself on their television "+
        "screens. What fools.. how I pity them. And yes, by the way, i DO have a Rick & "+
        "Morty tattoo. And no, you cannot see it. It's for the ladies' eyes only- and even "+
        "then they have to demonstrate that they're within 5 IQ points of my own (preferably "+
        "lower) beforehand. Nothin personnel kid",
    "jojo-reference":
        "!! !! !! HOLY FUCKING SHIT !! !! !! !! IS THAT A MOTHERFUCKING JOJO REFERENCE "+
        "?!? ?!? ?!? JOJO IS THE BEST FUCKING ANIME !!1!1!1!1! JOTARO IS SO BADASSSSS  "+
        "ORAORA ORAORA ORAORA ORAORA ORAORA ORA ORA ORA ORAORA ORA ORA ORA ORA ORA MUDA "+
        "MUDA MUDAMUDA MUDAMUDA MUDA MUDA MUDA MUDA MUDA MUDA MUDA MUDA MUDA MUDA MUDA MUDA "+
        "MUDAMUDA MUDAMUDAMUDAMUDA MUDA MUDAMUDAMUDA "+
        "WRYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY "+
        "Yo Angelo!Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo "+
        "Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo"+
        " Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo "+
        "Angelo! Yo Angelo! Yo Angelo! Yo Angelo!Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo "+
        "Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Yo Angelo! Oh you're approaching me ? ? ? "+
        "But it was me, Dio !! !! !! !! !!",
    "darth-plagueis":
        "Did you ever hear the tragedy of Darth Plagueis The Wise? I thought not. It's "+
        "not a story the Jedi would tell you. It's a Sith legend. Darth Plagueis was a "+
        "Dark Lord of the Sith, so powerful and so wise he could use the Force to "+
        "influence the midichlorians to create life... He had such a knowledge of the dark "+
        "side that he could even keep the ones he cared about from dying. The dark side "+
        "of the Force is a pathway to many abilities some consider to be unnatural. He "+
        "became so powerful... the only thing he was afraid of was losing his power, which "+
        "eventually, of course, he did. Unfortunately, he taught his apprentice "+
        "everything he knew, then his apprentice killed him in his sleep. Ironic. He "+
        "could save others from death, but not himself.",
    "a-very-particular":
        "I don't know who you are. I don't know what you want. If you're looking for "+
        "ransom, I can tell you I don't have money but what I do have are a very "+
        "particular set of skills. Skills I have acquired over a very long career. "+
        "Skills that make me a nightmare for people like you. If you let my daughter go "+
        "now, that will be the end of it. I will not look for you, I will not pursue "+
        "you. But if you don't, I will look for you, I will find you and I will kill you.",
    "c++":
        "*YOU* are full of bullshit.\n"+
        "\n"+
        "C++ is a horrible language. It's made more horrible by the fact that a lot of "+
        "substandard programmers use it, to the point where it's much much easier to "+
        "generate total and utter crap with it. \nQuite frankly, even if the choice of C "+
        "were to do *nothing* but keep the C++ programmers out, that in itself would be "+
        "a huge reason to use C.\n"+
        "\n"+
        "In other words: the choice of C is the only sane choice. I know Miles Bader "+
        "jokingly said \"to piss you off\", but it's actually true. I've come to the "+
        "conclusion that any programmer that would \nprefer the project to be in C++ "+
        "over C is likely a programmer that I really *would* prefer to piss off, so that "+
        "he doesn't come and screw up any project I'm involved with.\n"+
        "\n"+
        "C++ leads to really really bad design choices. You invariably start using the "+
        "\"nice\" library features of the language like STL and Boost and other total and "+
        "utter crap, that may \"help\" you \nprogram, but causes:\n"+
        "\n"+
        " - infinite amounts of pain when they don't work (and anybody who tells me that "+
        "STL and especially Boost are stable and portable is just so full of BS that "+
        "it's not even funny)\n"+
        "\n"+
        " - inefficient abstracted programming models where two years down the road you "+
        "notice that some abstraction wasn't very efficient, but now all your code "+
        "depends on all the nice object models \naround it, and you cannot fix it "+
        "without rewriting your app.\n"+
        "\n"+
        "In other words, the only way to do good, efficient, and system-level and "+
        "portable C++ ends up to limit yourself to all the things that are basically "+
        "available in C. And limiting your project to C \nmeans that people don't screw "+
        "that up, and also means that you get a lot of programmers that do actually "+
        "understand low-level issues and don't screw things up with any idiotic \"object "+
        "model\" crap.\n"+
        "\n"+
        "So I'm sorry, but for something like git, where efficiency was a primary "+
        "objective, the \"advantages\" of C++ is just a huge mistake. The fact that we "+
        "also piss off people who cannot see that is \njust a big additional advantage.\n"+
        "\n"+
        "If you want a VCS that is written in C++, go play with Monotone. Really. They "+
        "use a \"real database\". They use \"nice object-oriented libraries\". \n"+
        "They use \"nice C++ abstractions\". And quite frankly, as a result of all these "+
        "design decisions that sound so appealing to some CS people, the end \n"+
        "result is a horrible and unmaintainable mess.\n"+
        "\n"+
        "But I'm sure you'd like it more than git.\n"+
        "\n"+
        "            Linus\n"
};
