/* help.c */
/**/

#include "imoria.h"
#include <signal.h>
#include <sys/wait.h>
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void help()
{
  /*{ Help for available commands                                       }*/

  char      command;

  inkey_delay(&command,0);
  if (command == 0) {
      clear_screen();
prt("A       Age of character.     |  h       Hurl an item.",1,1);
prt("B <Dir> Bash (object/creature)|  i       Inventory list.",2,1);
prt("C       Display character.    |  j <Dir> Jam a door with spike.",3,1);
inkey_delay(&command,0);
if (command == 0) {
prt("D <Dir> Disarm a trap/chest.  |  l <Dir> Look given direction.",4,1);
prt("E       Eat some food.        |  m       Cast a magic spell.",5,1);
prt("F       Fill lamp with oil.   |  m       Use a music book.",6,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("G       Game time and date    |  o <Dir> Open a door/chest.",7,1);
prt("H       Help                  |  p       Read a prayer.",8,1);
prt("I       Inven of one item type|  p       Play an instrument.",9,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("L       Current location.     |  q       Quaff a potion.",10,1);
prt("M       Money.                |  r       Read a scroll.",11,1);
prt("P       Print map.            |  s       Search for hidden doors.",12,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("R       Rest for a period.    |  t       Take off an item.",13,1);
prt("S       Search Mode.          |  u       Use a staff.",14,1);
prt("T <Dir> Tunnel.               |  v       Version and credits.",15,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("U       Use miscellaneous item|  w       Wear/Wield an item.",16,1);
prt("W       Current time and date |  x       Exchange weapon.",17,1);
prt("a       Aim and fire a wand.  |  c <inv> Clean inventory.",18,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("b       Browse a book.        |  $       Shell out of game.",19,1);
prt("c <Dir> Close a door.         |  +       Experience for levels.",20,1);
prt("d       Drop an item.         |  . <Dir> Move in direction.",21,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("e       Equipment list.       |  /       Identify a character.",22,1);
prt("f       Fire Projectile.      |  X       Toggle light source.",23,1);

      pause_game(24);
}
clear_screen();
inkey_delay(&command,0);
if (command == 0) {
prt("<       Go up an up-staircase.|   >      Go down a down-staircase.",1,1);
prt("?       Display this panel.   |  ^M      Repeat the last message.",2,1);
prt("]       Armor list.           |  ^R      Redraw the screen.",3,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("|       Weapon list.          |  ^C      Quit the game.",4,1);
prt("J       Map shown reduced size|  ^Y      Save character and quit.",5,1);
prt("Movement:",7,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("          7  8  9",8,1);
prt("          4     6    5 = Rest",9,1);
prt("          1  2  3",10,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("Directory of Shops:",12,1);
prt("     A   Alchemy Shop                M   Magic Shop",13,1);
prt("     B   Bank                        P   Trading Post",14,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("     C   Casino                      Q   Questor's Home",15,1);
prt("     D   All-Nite Deli               R   Armory",16,1);
prt("     G   General Store               T   Temple",17,1);
inkey_delay(&command,0);
}
if (command == 0) {
prt("     I   Inn                         U   Music Shop",18,1);
prt("     J   Gem Shop                    W   Weapon Smith",19,1);
prt("     L   Library                     X   Money Exchange",20,1);
prt("     +   Unknown",21,1);
      pause_game(24);
}
draw_cave();
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void wizard_help()
{
  /*{ Help for available wizard commands                            }*/

      clear_screen();
      prt("^A -  Remove Curse and Cure all maladies.",1,1);
      prt("^B -  Print random objects sample.",2,1);
      prt("^D -  Down/Up n levels.",3,1);
      prt("^E - *Change character.",4,1);
      prt("^F - *Delete monsters.",5,1); 
      prt("^G - *Allocate treasures.",6,1);
      prt("^H -  Wizard Help.",7,1);
      prt("^I -  Identify.",8,1);
      prt("^J - *Gain experience.",9,1);
      prt("^K - *Summon monster.",10,1);
      prt("^L -  Wizard light.",11,1);
      prt("^N -  Print monster dictionary.",12,1);
      prt("^O - *Summon monster by its name.",13,1);
      prt("^P -  Wizard password on/off.",14,1);
      prt(" s - *Statistics on item (in inventory screen).",15,1);
      prt("^T -  Teleport player.",16,1);
      prt("^U - *Roll up an item.",17,1);
      prt("^V -  Restore lost character.",18,1);
      prt("^W - *Create any object *CAN CAUSE FATAL ERROR*",19,1);
      prt("^X - *Edit high score file",20,1);
      pause_game(24);
      draw_cave();
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void moria_help(vtype help_level)
{
  ntype     help_string;
  pid_t     help_pid;
  int       status;

  game_state = GS_HELP;

  sprintf(help_string, "Imoria %s", help_level);

  if ((help_pid = fork()) == 0) {
    /* child process */
    echo();
    nocbreak();
    clear_screen();
    put_qio();
    endwin();
    execlp(HELP_FILE_PATH, "mhelp.pl", help_string, 0);
    printf("\n\rUnable to execute: %s\n\r", HELP_FILE_PATH);
    printf("Press return continue.\n\r");
    fflush(stdout);
    getc(stdin);
    exit(1);
  } else if (help_pid == -1) {
    /* parent process on failure */
    msg_print("Fork failed.");
  } else {
    /* parent process on success */

#if DO_DEBUG
    fprintf(debug_file,": child: %d  watiting...\n",help_pid);
    fflush(debug_file);
#endif      
    
    waitpid(help_pid, &status, 0);  /* mhelp.pl catches ^Z and exits */
    
#if DO_DEBUG
    fprintf(debug_file,": child: %d  status 1: %d\n",
	    help_pid, status);
    fflush(debug_file);
#endif      

    waitpid(help_pid, &status, 0);  /* mhelp.pl catches ^Z and exits */
    
#if DO_DEBUG
    fprintf(debug_file,": child: %d  status 2: %d\n",
	    help_pid, status);
    fflush(debug_file);
#endif      

    cbreak();
    noecho();
    clear_screen();
    draw_cave();
  }

  //printf("\nDisplay help for %s\n\r", help_level);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void ident_char()
{
  char      command;
  boolean   exit_flag,bleah_flag;

  do {
    exit_flag = false;
    bleah_flag = true;
    msg_print("Identify letter for what environ? (<t>own, <d>ungeon, <w>ater) ");
    if (get_com("",&command)) {
      switch (command) {
      case 't': case 'd': case 'w': 
        exit_flag = true;
        break;
      }
    } else {
      bleah_flag = false;
      exit_flag  = true;
    }
  } while (!exit_flag);

  if (bleah_flag) {
    switch (command) {
    case 't' : 
      if (get_com("Enter character to be identified : ",&command)) {
        switch (command) {
        case '#' : prt("# - A stone wall.",1,1);break;
        case '+' : prt("+ - Entrance to a building.",1,1);break;
        case '.' : prt(". - Ground.",1,1);break;
        case '>' : prt("> - A down staircase.",1,1);break;
        case '@' : prt(py.misc.name,1,1);break;
        case 'p' : prt("p - A townsperson.",1,1);break;
        case 'A' : prt("A - Alchemy Shop.",1,1);break;
        case 'B' : prt("B - First Moria National Bank.",1,1);break;
        case 'C' : prt("C - Casino.",1,1);break;
        case 'D' : prt("D - All-Nite Deli",1,1);break;
        case 'G' : prt("G - General Store.",1,1);break;
        case 'I' : prt("I - Inn.",1,1);break;
        case 'J' : prt("J - Gem and Jewelry Store.",1,1);break;
        case 'L' : prt("L - Library.",1,1);break;
        case 'M' : prt("M - Magic Shop.",1,1);break;
        case 'N' : prt("N - Insurance Shop.",1,1);break;
        case 'P' : prt("P - Trading Post.",1,1);break;
        case 'Q' : prt("Q - Home of the Questor.",1,1);break;
        case 'R' : prt("R - Armory.",1,1);break;
        case 'T' : prt("T - Temple.",1,1);break;
        case 'U' : prt("U - Music Shop.",1,1);break;
        case 'W' : prt("W - Weapon Smith.",1,1);break;
        case 'X' : prt("X - Money Exchange.",1,1);break;
        default  : prt("Not normally used.",1,1);break;
        }
      }
      break;

    case 'd' : 
      if (get_com("Enter character to be identified : ",&command)) {
        switch (command) {
        case ' ' : prt("  - An open pit.",1,1);break;
        case '!' : prt("! - A potion.",1,1);break;
        case '"' : prt("\" - An amulet, periapt, or necklace.",1,1);break;
        case '#' : prt("# - A stone wall.",1,1);break;
        case '$' : prt("$ - Treasure.",1,1);break;
        case '%' : prt("% - A musical instrument or song book.",1,1);break;
        case '&' : prt("& - Treasure chest.",1,1);break;
        case '\'': prt("' - An open door.",1,1);break;
        case '(' : prt("( - Soft armor.",1,1);break;
        case ')' : prt(") - A shield.",1,1);break;
        case '*' : prt("* - Gems or Jewelry.",1,1);break;
        case '+' : prt("+ - A closed door.",1,1);break;
        case ',' : prt(", - Food or mushroom patch.",1,1);break;
        case '-' : prt("- - A wand",1,1);break;
        case '.' : prt(". - Floor.",1,1);break;
        case '/' : prt("/ - A pole weapon.",1,1);break;
        case ':' : prt(": - Rubble.",1,1);break;
        case ';' : prt("; - A loose rock.",1,1);break;
        case '<' : prt("< - An up staircase.",1,1);break;
        case '=' : prt("= - A ring.",1,1);break;
        case '>' : prt("> - A down staircase.",1,1);break;
        case '?' : prt("? - A scroll.",1,1);break;
        case '@' : prt(py.misc.name,1,1);break;
        case 'A' : prt("A - Giant Ant Lion.",1,1);break;
        case 'B' : prt("B - Demon.",1,1);break;
        case 'C' : prt("C - Gelentanious Cube.",1,1);break;
        case 'D' : prt("D - An Ancient Dragon (Beware).",1,1);break;
        case 'E' : prt("E - Elemental.",1,1);break;
        case 'F' : prt("F - Giant Fly or Faerie Dragon.",1,1);break;
        case 'G' : prt("G - Ghost.",1,1);break;
        case 'H' : prt("H - Hobgoblin.",1,1);break;
        case 'J' : prt("J - Jelly.",1,1);break;
        case 'K' : prt("K - Killer Beetle.",1,1);break;
        case 'L' : prt("L - Lich.",1,1);break;
        case 'M' : prt("M - Mummy.",1,1);break;
        case 'N' : prt("N - Nymph",1,1);break;
        case 'O' : prt("O - Ooze.",1,1);break;
        case 'P' : prt("P - Giant humanoid.",1,1);break;
        case 'Q' : prt("Q - Quylthulg (Pulsing Flesh Mound).",1,1);break;
        case 'R' : prt("R - Reptile.",1,1);break;
        case 'S' : prt("S - Giant Scorpion/Sandgorgon.",1,1);break;
        case 'T' : prt("T - Troll.",1,1);break;
        case 'U' : prt("U - Umber Hulk.",1,1);break;
        case 'V' : prt("V - Vampire.",1,1);break;
        case 'W' : prt("W - Wight or Wraith.",1,1);break;
        case 'X' : prt("X - Xorn.",1,1);break;
        case 'Y' : prt("Y - Yeti.",1,1);break;
        case 'Z' : prt("Z - Nazgul",1,1);break;
        case '[' : prt("[ - Hard armor.",1,1);break;
        case '\\' : prt("\\ - A hafted weapon.",1,1);break;
        case ']' : prt("] - Misc. armor.",1,1);break;
        case '^' : prt("^ - A trap.",1,1);break;
        case '_' : prt("_ - A staff.",1,1);break;
        case '`' : prt("` - Water.",1,1);break;
        case 'a' : prt("a - Amphibian.",1,1);break;
        case 'b' : prt("b - Giant Bat.",1,1);break;
        case 'c' : prt("c - Insect.",1,1);break;
        case 'd' : prt("d - Dragon.",1,1);break;
        case 'e' : prt("e - Floating Eye.",1,1);break;
        case 'f' : prt("f - Fish.",1,1);break;
        case 'g' : prt("g - Golem.",1,1);break;
        case 'h' : prt("h - Harpy.",1,1);break;
        case 'i' : prt("i - Icky Thing.",1,1);break;
        case 'j' : prt("j - Canine.",1,1);break;
        case 'k' : prt("k - Kobold.",1,1);break;
        case 'l' : prt("l - Giant Lice.",1,1);break;
        case 'm' : prt("m - Mold.",1,1);break;
        case 'n' : prt("n - Naga.",1,1);break;
        case 'o' : prt("o - Orc or Ogre.",1,1);break;
        case 'p' : prt("p - Person (Humanoid).",1,1);break;
        case 'q' : prt("q - Quasit.",1,1);break;
        case 'r' : prt("r - Rodent.",1,1);break;
        case 's' : prt("s - Skeleton.",1,1);break;
        case 't' : prt("t - Giant tick.",1,1);break;
        case 'v' : prt("v - Swirling Vapor.",1,1);break;
        case 'w' : prt("w - Worm(s).",1,1);break;
        case 'x' : prt("x - Spider.",1,1);break;
        case 'y' : prt("y - Yeek.",1,1);break;
        case 'z' : prt("z - Zombie.",1,1);break;
        case '{' : prt("{ - Arrow, bolt, or bullet.",1,1);break;
        case '|' : prt("| - A sword or dagger.",1,1);break;
        case '}' : prt("} - Bow, crossbow, or sling.",1,1);break;
        case '~' : prt("~ - Miscellaneous item.",1,1);break;
        default  : prt("Not Used.",1,1);
        }
      }
      break;

    case 'w' : 
      if (get_com("Enter character to be identified : ",&command)) {
        switch (command) {
        case 'a' : prt("a  - Amphibian.",1,1);break;
        case 'f' : prt("f  - Fish.",1,1);break;
        case 'l' : prt("l  - Leach.",1,1);break;
        case 'm' : prt("m  - Man-o-War.",1,1);break;
        case 'w' : prt("w  - Kelp.",1,1);break;
        case 'C' : prt("C  - Alligator or Crocodile.",1,1);break;
        case 'D' : prt("D  - Dinosaur.",1,1);break;
        case 'H' : prt("H  - Hippo.",1,1);break;
        case 'R' : prt("R  - Remora.",1,1);break;
        case 'S' : prt("S  - Shark.",1,1);break;
        default  : prt("Not Used.",1,1);
        }
      }
      break;
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/* END FILE  help.c */
