#!/usr/bin/perl
###########################################################################
#
# Read a VMS runoff file that was destined to be a help file.
# Only handles directives that are in the moria help file, and
# I never found out what all the values really mean.
#
# Steve Kertes, December 1997.
# Version 1.1
#
###########################################################################

$SIG{'TSTP'} = 'bailout';
#$SIG{'INT'} = 'bailout';

($ispeed,$ospeed) = (5,5);
require 'termcap.pl';

$total_rows = get_rows();

sub get_rows {
	&Tgetent($ENV{'TERM'});	# sets $TC{'cm'}, etc.
 	if (exists($TC{'li'})) {
	    return $TC{'li'};
 	} elsif (exists($ENV{'LINES'})) {
	    return $ENV{'LINES'};
 	} else {
	    return 24;          # a reasonable default
 	}

}
##############################

@search_topic    = @ARGV;
@ARGV            = ();
@search_topic    = ("Imoria") if ($#search_topic == -1);

if ($#search_topic == 0) {
    @search_topic = split(' ',$search_topic[0])
}

##############################

$| = 1;

$num_lines       = $total_rows - 2;
$topic_width     = 65;

$left_margin     = 0;
$right_margin    = 70;
$literal         = 0;

$topic_count     = 1;
@topic_array     = ();
@depth_array     = ();
@section_array   = ();

$current_topic   = "Imoria";
$current_depth   = 0;
$current_section = ".lm $left_margin\n.rm $right_margin\n.p\nThe game of Imoria is a single player dungeon simulation. The object is to decend deep into the vaults of Moria to  slay the evil Balrog, ruler of Moria.  This is a special  improved version of the original Moria and includes many new  objects and monsters, as well as an enlarged Town level and a few new classes and races to pit against the dungeon's inhabitants.\n.p\nInformation about the game is found in this VMS style help. You do not need to read everything in here, but there really is some useful stuff!\n.lm 0\n";


##############################

parse_topics();
#print_all_topics();

$show_help = 1;
$first_help_ever = 1;   # we do not want to exit if the initial search fails

SEARCHLOOP:
for ( ; ($#search_topic >= 0) ; ) {

    #
    # pop topics off the search list until we find a match.
    # If we run out of topics and this is the first search
    # then default to Imoria, otherwise bail.
    #
    for (;;) {
	$found = find_topic(0, @search_topic);
	last if ($found != -1);
	$show_help = 0;
	pop(@search_topic);
	if ($#search_topic < 0) {
	    last SEARCHLOOP unless $first_help_ever;
	    $show_help = 1;
	    @search_topic = ("Imoria");
	}
    }

    $first_help_ever = 0;
    $help_text = "";

    #
    # when popping topics off the search list we
    # usually do not want to see all the help text
    #
    if ($show_help) {
	hprint ("=" x 70 . "\n");
	hprint ("\t\t" . $topic_array[$found] . "\n\n");
	hprint_selection($section_array[$found]);
    }

    #
    # print out the sub topics that can be reached from here
    #
    @sub_topics = find_sub_topics($found);
    #hprint ("-" x 70 . "\n");
    if ($#sub_topics >= 0) {
	$saved_topic = "";
	hprint ("Additional information available:\n \n");
	hprint_sub_topics(@sub_topics);
    } else {
	# no subtopics exist, so automaticly pop off a level
	# but save it so "?" still works (commented out)
	$saved_topic = pop(@search_topic);
	$found = find_topic(0, @search_topic);
	@sub_topics = find_sub_topics($found);
	hprint (" \nAdditional information available:\n \n");
	hprint_sub_topics(@sub_topics);
    }
    
    #
    # display the help text
    #
    @help_lines = split('\n', $help_text);
    
    for ($x = 0; $x <= $#help_lines; $x++) {
	$x = 0 if ($x < 0);
	
	print $help_lines[$x] . "\n\r";
	
	if (($x+1) % $num_lines == 0){
	    print "\n\r(Press return for more)";
	    chomp($y = <>);
	    if ($y eq "b") {
		$x -= $num_lines * 2;
	    } elsif ($y eq "a") {
		
	    }
	    print "\n\r";
	}
    }

    #
    # get the next command
    #
    if ($#sub_topics >= 0) {
	if ($found == 0) {
	    print "\n\r" . (uc $search_topic[$#search_topic]) . " Topic? ";
	} else {
	    print "\n\r" . (uc $search_topic[$#search_topic]) . " Subtopic? ";
	}
    } else {
	print "\n\rNo additional information.  Press return to continue: ";
    }

    chomp($y = <>);
    if ($y eq "?") {
	#push (@search_topic, ( $saved_topic )) if ($saved_topic ne "");
	$show_help = 1;
    } elsif ($y eq "") {
	# back up one level
	pop  (@search_topic);
	$show_help = 0;
    } else {
	# try to find the new topic, try for exact match first, then
	# accept partial matches
	$exact   = "";
	$partial = "";
	foreach $q (@sub_topics) {
	    if ($topic_array[$q] =~ /^$y$/i) {
		$exact = $topic_array[$q];
		last;
	    } elsif (($partial eq "") && ($topic_array[$q] =~ /^$y/i)) {
		$partial = $topic_array[$q];
	    }

	}
	$w = $exact;
	$w = $partial if ($w eq "");
	$w = $y       if ($w eq "");
	push (@search_topic, ( $w ));
	$show_help = 1;
    }

}

###########################################################################
###########################################################################
###########################################################################
sub bailout {
#
# Handler for signals, first argument is signal name
#
    #local($sig) = @_;
    #print "Caught a SIG$sig--shutting down\n\r";
    #print "\n\r";
    exit(0);
}  
###########################################################################
sub parse_topics {
while (<DATA>) {
    if($literal) {
	# check to see if it gets out of literal mode
	# and then copy the line

	if ( /^(\.el)|(\.end\s+literal)/ ) {
	    $literal = 0;
	}

	add_line();

    } else {
	# check to see if a new section has begin, otherwise
	# just keep on copying, look out for literal mode

	if ( /^(\d+)\s+(.+)$/ ) {
	    # hey! a new section has begun!
	    $new_depth = $1;
	    $new_topic = $2;

	    end_topic();
	    start_topic($new_depth, $new_topic);

	} elsif ( /^\.literal/ ) {
	    # begin literal section
	    $literal = 1;
	    add_line();
	} else {
	    # nothing special...
	    parse_margins();
	    add_line();
	}
    }
}
end_topic();
}
###########################################################################
sub end_topic {
    local($x);


    $topic_count++;
    push @topic_array,   ($current_topic);
    push @depth_array,   ($current_depth);
    push @section_array, ($current_section);
    
    $current_topic   = "";
    $current_depth   = 0;
    $current_section = "";
}
###########################################################################
sub start_topic {
    local($depth, $topic) = @_;

    $current_topic   = $topic;
    $current_depth   = $depth;
    $current_section = ".lm $left_margin\n.rm $right_margin\n";
}
###########################################################################
sub add_line {
    $current_section .= $_;
}
###########################################################################
sub parse_margins {
    if ( /\.lm\s*(\d+)/ ) {
	$left_margin = $1;
    }

    if ( /\.rm\s*(\d+)/ ) {
	$right_margin = $1;
    }
}
###########################################################################
sub print_all_topics {
    for($x=0; $x < $topic_count; $x++) {
     print "============================\n\r";
     print ((' ' x ($depth_array[$x]*4)) . "" . $topic_array[$x] . "\n\r\n\r");
#    print $section_array[$x];
    }
}
###########################################################################
sub find_topic {
    local($find_partial, @s) = @_;
    local($return_val, $found, $x, $y, $z);

    $found = 0;
    $cur_depth = 0;

    for ($y = 0; ($y<$topic_count) && (!$found); $y++) {
	if (($cur_depth == $depth_array[$y])
	    && ($s[$cur_depth] eq $topic_array[$y]))
	{
	    $return_val = $y;
	    if (++$cur_depth > $#s) {
		$found = 1;
	    }
	} elsif ($cur_depth > $depth_array[$y+1]) {
	    #print ">>>bailing on curdepth $cur_depth  y = $y<<<\n\r";
	    last;
	}
    }

    $found ? $return_val : ($find_partial ? $return_val : -1);
}
###########################################################################
sub find_sub_topics {
    local($x) = @_;
    local($y, $z, @ret_value);

    $y = $depth_array[$x]; 
    for ($z = $x+1; ($z < $topic_count) && ($depth_array[$z] > $y); $z++) {
	if ($depth_array[$z] == $y+1) {
	    push @ret_value, ($z);
	}
    }

    @ret_value;
}
###########################################################################
sub hprint_sub_topics {
    local(@s) = @_;
    local($x, $one_line, $l);

    $one_line = "";
    $l = 0;
    foreach $x (@s) {
	if (($l == 0) || (($l+length($topic_array[$x])) <= $topic_width)) {
	    $one_line .= "   " unless ($l == 0);
	    $one_line .= $topic_array[$x];
	    $l = length($one_line);
	} else {
	    hprint(" " x 3 . $one_line . "\n");
	    $one_line = $topic_array[$x];
	    $l = length($one_line);
	}
    }
    hprint(" " x 3 . $one_line . "\n") unless (length($one_line) == 0);
}
###########################################################################
sub hprint_selection {
    local($s) = @_;
    local($lm, $rm, @lines, $inList, $inLiteral, $inPargraph);
    local($listMarker, $printingStuff);
    local($x, $y, $z);

    @lines = split('\n', $s);
    LINE: for ($x = 0; $x <= $#lines; $x++) {

	$_ = $lines[$x];

	    # left margin
	    if ( /\.lm\s+(\d+)/ ) {
		$lm = (' ' x $1);
	    }

	    # right margin
	    if ( /\.rm\s+(\d+)/ ) {
		$rm = (' ' x $1);
	    }

	    # blank lines
	    if ( /\.blank\s+(\d+)/ ) {
#		print "BLANK $1";
		for ($y = 0; $y < $1; $y++) {
		    hprint("\n");
		}
	    }

	    # list
	    if ( /\.list\s+(\d+)\s+'(.)'/ ) {
#		print "Start List >>$1<< >>$2<<\n\r";
		$listmarker = $2;
		$listline = "";
		for ( ; $x <= $#lines; $x++) {
		    last if ($lines[$x] =~ /\.le;/);
		}
		for ( ; $x <= $#lines; $x++) {
		    $partial = $lines[$x];
		    $partial =~ s/^(\.le;)*\s*/ /;
		    $listline .= $partial;
		    if ( $lines[$x+1] =~ /(\.els)|(\.le;)/ ) {
			hemit_listitem($lm, $rm, $listmarker, $listline);
			$listline = "";
		    }

		    last if ( $lines[$x+1] =~ /\.els/);
		}
		hprint("\n");
		next LINE;
	    }

	    # literal
	    if ( /\.literal/ ) {
#		print "Start Literal\n\r";
		for ($x++ ; $x <= $#lines; $x++) {
		    hprint($lines[$x] . "\n");
		    last if ( $lines[$x+1] =~ /^(\.el)|(\.end\s+literal)/);
		}
		next LINE;
	    }

	    # paragraph
	    if ( /\.p/ ) {
#		print "Start Paragraph\n\r";
		$para = "";
		for ($x++ ; $x <= $#lines; $x++) {
		    $para .= $lines[$x];
		    last if ( $lines[$x+1] =~ /^\./);
		}
		hemit_paragraph($lm, $rm, $para);
		hprint("\n") if ($para =~ /\S/);
		next LINE;
	    }

	    # unbound paragraph, anything without a . as the first char
	    if ( !(/^\./) ) {
#		print "Start Unbound Paragraph\n\r";
		$para = "";
		for (; $x <= $#lines; $x++) {
		    $para .= $lines[$x];
		    last if ( $lines[$x+1] =~ /^\./);
		}
		hemit_paragraph($lm, $rm, $para);
		hprint("\n") if ($para =~ /\S/);
		next LINE;
	    }

    } # end for
}
###########################################################################
sub hemit_paragraph {
    local($lm, $rm, $s) = @_;
    local($x, $y, $one_line);

    $y = length($rm) - length($lm);
    $one_line = ""; $x = 0;

    $s =~ s/\n +/ /g;
    $s =~ s/(\n|\t)/ /g;
    foreach $word (split(' ',$s)) {
	if (($x == 0) || (($x+length($word)) <= $y)) {
	    $one_line .= " " unless ($x == 0);
	    $one_line .= $word;
	    $x = length($one_line);
	} else {
	    hprint($lm . $one_line . "\n");
	    $one_line = "$word "; $x = length($one_line);
	}
    }
    hprint($lm . $one_line . "\n");
}

###########################################################################
sub hemit_listitem {
    local($lm, $rm, $marker, $s) = @_;
    local($x, $y, $one_line);

    $y = length($rm) - length($lm);
    $one_line = "$marker "; $x = length($one_line);

    $s =~ s/\n +/ /g;
    $s =~ s/(\n|\t)/ /g;

    foreach $word (split(' ',$s)) {
	if (($x == 0) || (($x+length($word)) <= $y)) {
	    $one_line .= " " unless ($x == 0);
	    $one_line .= $word;
	    $x = length($one_line);
	} else {
	    hprint($lm . $one_line . "\n");
	    $one_line = "   $word ";
	    $x = length($one_line);
	}
    }
    hprint($lm . $one_line . "\n");

}
###########################################################################
sub hprint {
    local($x) = @_;
    $help_text .= $x;
}
###########################################################################

__END__
.no paging
.rm 70
.lm 1
.lm 0 .blank 2
1 Abbreviations
.lm 1
.p
      Sometimes  abbreviations  are  used to  indicate  especially
 powerful items.  These abbreviations  are usually used with armor
 and  weapons.  For  more detailed  information  on them,  see the
 sections entitled Weapons and Armor.
.blank 2
.lm 0
2 Weapons
 
.lm 15
.blank 2 .list  0 ' '
.le;           (DB) - Demon Bane.
.le;           (DF) - Defender.
.le;           (FB) - Frost Brand.
.le;           (FT) - Flame Tongue.
.le;           (HA) - Holy Avenger.
.le;           (SD) - Slay Dragon.
.le;           (SE) - Slay Evil.
.le;           (SM) - Slay Monster.
.le;           (SR) - Slay Regenerative.
.le;           (SS) - Soul Sword.
.le;           (SU) - Slay Undead.
.le;           (V)  - Vorpal Weapon.
.le;           (WB) - Wizard Blade.
.le;           (BB) - Blessed Blade.
.els
.lm 0 .blank 2
2 Armor
.lm 15
.blank 2
.list 0 ' '
.le;           (RA) - Resist Acid.
.le;           (RC) - Resist Cold.
.le;           (RF) - Resist Fire.
.le;           (RL) - Resist Lightning.
.le;           (R)  - Resistance, resists all of the above.
.els
.lm 0
.blank 2
1 Adventuring
.lm 1
.p
      Once your character is adequately supplied with food, light,
 armor,  and  weapons,  he is ready to enter the dungeon.  Move on
 top of the '>'  symbol  and  use  the  down  '>'  command.   Your
 character enters a maze of interconnecting staircases and finally
 passes through a one-way door.  He is now on in the dungeon somewhere
 between 50 and 200 feet depending on the steepness of the stairs.
 He must  survive  many  horrible and
 challenging encounters to find the treasure lying about.
.lm 0
.blank 2
.lt
2 Armor_Class
.el
.lm 1
.p
      Armor class is a number that describes the  amount  and  the
 quality of armor begin worn.
.p
      The larger your armor class, the more protective it  is.   A
 negative  armor  class  would  actually  help get you hit.  Armor
 protects you in three manners:
.blank 2
.list 0 'o'
 
.le;      It makes you less likely to be hit by the monsters.
.le;      You take less damage when you DO get hit.
.le;      Fire and Acid damage are reduced when wearing body armor.
.els
.lm 1
.p
 Obviously a high  armor class is a must  for surviving the  lower
 levels of Moria.
.p
      Each piece of armor has an armor  class  adjustment,  and  a
 magical  bonus.   Armor  bought  in  town  will have these values
 displayed with its  description.  Armor  that is found within the
 dungeon must be identified before these values will be displayed.
 Armor class values are always diplayed between a set of  brackets
 '[#,+#]'.   The  first value is the armor class of the item.  The
 second number is the magical bonus of the item, and  will  always
 have a sign preceding the value.  For example, an item listed  as
 [3,+2] would increase your armor class by 5 points (3+2) if worn.
 There are a few cases where  the form '[+#]' is used, meaning the
 object  itself gives no  protection, but  it does grant a magical
 bonus to your armor class if worn.
.p
 
      Some pieces of armor will possess special  abilites  denoted
 by the following abbreviations:
.lm 15
.blank 2
.list 0 ' '
 
.le;      (RA) - Resist Acid.
.els
.lm 1
.p
 
          This magical ability is usually  enchanted  into  armor,
     but  may  occasionally be found as an ability of a weapon.  A
     character using  such  an  object  will  take  only  one half
     damage  from any acid thrown upon him.  In addition, armor so
     enchanted will resist the acid's effects and not  be  damaged
     by it.
.lm 15
.blank 2
.list 0 ' '
 
.le;      (RC) - Resist Cold.
.els
.lm 1
.p
          This magical ability is also found in both  weapons  and
     armor.  A character using a resist cold object will take only
     one third damage from frost and cold.
.lm 15
.blank 2
.list 0 ' '
.le;      (RF) - Resist Fire.
.els
.lm 1
.p
          This magical ability is found in both weapons and armor.
     A  character  using  a  resist fire object will take only one
     third damage from heat and fire.
.lm 15
.blank 2
.list 0 ' '
.le;      (RL) - Resist Lightning.
.els
.lm 1
.p
 
          This magical ability is found in both weapons and armor.
     A  character  using  a resist lightning object will take only
     one third damage from electrical attacks.
.lm 15
.blank 2
.list 0 ' '
.le;      (R) - Resistance.
.els
.lm 1
.p
 
     A character wearing   armor   with  this  ability  will  have
     resistance to Acid, Cold, Fire, and Lightning as explained in
     each part above.
.lm 0
.blank 2
.lt
3 Armor_List
.el
.lm 1
.literal
                                  Cost   Weight   +To AC   Lvl
 -------------------------------------------------------------
 Pair of Sandals                    1       1        0      1
 Pair of Soft Leather Shoes         4       5        1      1
 Pair of Soft Leather Boots         7      20        2      4
 Pair of Hard Leather Boots        12      40        3      6
 
 Cloth Hat                          5       5        0      1
 Soft Leather Cap                   4      10        1      2
 Hard Leather Cap                  12      15        2      4
 Metal Cap                         30      20        3      7
 Iron Helm                         75      75        5     20
 Steel Helm                       200      60        6     40
 Silver Crown                     250      20        0     44
 Golden Crown                     500      30        0     47
 Jewel Encrusted Crown           1000      40        0     50
 
 Robe                               4      20        2      1
 Soft Leather Armor                18      80        4      2
 Soft Studded Leather              35      90        5      3
 Hard Leather Armor                55     100        6      5
 Woven Cord Armor                  45     150        6      7
 Hard Studded Leather             100     110        7      7
 Soft Leather Ring Mail           160     130        6     10
 Hard Leather Ring Mail           230     150        8     12
 Leather Scale Mail               330     140       11     14
 Metal Scale Mail                 430     250       13     24
 Rusty Chain Mail                   0     200       14(-8) 26
 Chain Mail                       530     220       14     26
 Double Chain Mail                630     260       15     28
 Augmented Chain Mail             675     270       16     30
 Bar Chain Mail                   720     280       18     34
 Metal Brigandine Armor           775     290       19     36
 Laminated Armor                  825     300       20     38
 Partial Plate Armor              900     260       22     42
 Lamellar Armor                   950     340       23     44
 Plate Armor                     1050     380       25     48
 Ribbed Plate Armor              1200     380       28     50
 
 Cloak                              3      10        1      1
 
 Set of Cloth Gloves                1       1        0      1
 Set of Leather Gloves              3       5        1      1
 Set of Gauntlets                  35      25        2     12
 
 Small Leather Shield              30      50        2      3
 Medium Leather Shield             60      75        3      8
 Small Metal Shield                50      65        3     10
 Large Leather Shield             120     100        4     15
 Medium Metal Shield              125      90        4     20
 Large Metal Shield               200     120        5     30
.el
 
.lm 0 .blank 2
2 Bashing
.lm 1
.p
      Weight  is  the  primary  factor  in  being  able  to   bash
 something,  but  strength  plays  a  role  too.  After bashing, a
 character may be off balance for several  rounds  depending  upon
 his DEXTERITY.
.p
      Doors can be broken down by bashing them.  Once  a  door  is
 bashed open, it is forever useless and cannot be closed.
.p
 
      Chests too may be  bashed  open,  but  be  warned  that  the
 careless  smashing  of a chest often ruins the contents.  Bashing
 open a chest will not disarm any traps it may contain,  but  does
 allow the strong and ignorant to see what's inside.
.p
 
      Finally, a creature may be bashed.  If a shield is currently
 being  worn,  the  bash is a shield bash and will do more damage.
 In either case, a bash may throw an opponent off  balance  for  a
 number of rounds, allowing a player to get in a free hit or more.
 If the player is thrown off-balance, his opponent  may  get  free
 hits on him.  This is a risky attack.
.p
.lm 0
.blank 2
.lt
2 Cursed_Objects
.el
.lm 1
.p
      Some objects, mainly armor and weapons, have had curses laid
 upon  them.   These  horrible  objects  will  look like any other
 normal item, but will  detract  from  your  character's stats  or
 abilities  if worn.  They will also be impossible to remove until
 a "Remove Curse" spell is used.
.p
 
      When a cursed item has been identified, an asterisk '*' will
 appear  next  to the inventory letter of the item.  If you should
 wear a cursed item, you will immediately know it  is  cursed  and
 again the asterisk will appear.
.lm 0
.blank 2
 
2 Death
.lm 1
.p
      If your character falls below 0 hit points, he has died  and
 cannot  be  restored.  A tombstone showing information about your
 character  will  be  displayed  with  the  option  to  print  the
 information to a  file.  You will also be asked if you would like
 to print his character sheet to a file.
.p
 After the tombstone, a list of high scores will appear.  The number
 listed can be specified in the command line, default is 20.
.p
.lm 0
.blank 2
2 Fighting
.lm 1
.p
      Attacking is simple in Moria.  If you move into a  creature,
 you  attack  him.   You  can  attack  from a distance by firing a
 missile, or by magical means such as aiming  a  wand.   Creatures
 attack in  the same way - if they move into you, they attack you.
 Some creatures can  cast spells from a distance, and  dragon-type
 creatures  can  breathe from  a  distance, but these are the only
 exceptions.
.p
      If you are wielding a weapon, the damage for the  weapon  is
 used  when you hit the creature.  If you are wielding no weapons,
 you get  two fist  strikes.  A very  strong  creature (such  as a
 half-troll) or a monk can do a lot  of damage with  its fists.  A
 character may have  both a  primary and a  secondary weapon.  The
 secondary weapon is kept on  the  belt, or on  the  shoulder  for
 immediate use. You can switch between your primary and  secondary
 weapons by  using the 'x' command.  Be sure  you are wielding the
 proper weapon when fighting.  Beating a dragon over the head with
 a bow or shovel will simply make him mad, and get you killed.
.p
      Missile weapons (such as bows) can be wielded, and then  the
 proper  missile (in  this case an arrow) can be fired  across the
 room into a target.  Missiles can  be  used  without  the  proper
 missile  weapon,  but used together they have a greater range and
 do far more damage.
.p
      Hits and misses are determined by comparing your class, your
 level, and the armor class of  the monster. (at a given  level of
 experience, Fighters hit the most  often, and Mages hit the least
 often).  A miss doesn't necessarily mean that you  didn't hit the
 monster, but that you failed to do any damage (perhaps due to the
 monster's armor).  A 'hit' is any attempted  swing that  succeeds
 in  doing damage.  Higher armor classes make it harder to inflict
 damage,  therefore  you  will  miss  more  often  when  attacking
 creatures with higher armor classes.
.lm 0
.blank 2
2 Light
.lm 1
.p
      There are two sources for  light once you enter the dungeon.
 Permanent light which has been magically placed within rooms, and
 light sources carried by the player.  If neither  is present, the
 character  will  be  unable to map or see any attackers.  Lack of
 light will also affect searching, picking locks, and disarming.
.p
      A character must wield a torch or lamp in  order  to  supply
 his own light.  Once a torch or lamp has 50 or less turns (moves)
 left before burning out, you will be reminded that your source of
 light  is about  to run  out.  This  message will  continue to be
 displayed  at random intervals.  Once a torch is burnt out, it is
 useless and can be  dropped.  A lamp or  lantern can  be refilled
 with oil by using  the Fill ('F') command.  You  must, of course,
 be carrying extra oil to do this. There are also rumors of  magic
 lanterns that run on magical charges instead of  oil,  but  these
 would be rare if you could find them at all.
.lm 0
.blank 2
2 Creatures
.lm 1 .p
      There are hundreds of  horrible  creatures in the  depths of
 Moria and as you get deeper the weaker ones are replaced by  more
 vicious foes. There are several categories of monsters,  although
 some creatures belong in more than one of these.
.lm 0
.blank 2
3 Dragons
.lm 1 .p
      These magical reptiles live far down in the lower layers  of
 the dungeon.  There are several  species,  each of  which  have a
 different shade of scales. As they get older they may learn magic
 with which to defend themselves, or just get an easy meal.  It is
 rumored that some of the dragons are born  with  bizzare  changes
 that are due to the magical nature of  dragons,  such as multiple
 heads or a highly magical breed. One thing is for certain
 and that is that the best weapon to fight them  with  is  a  Slay
 Dragon weapon; one that was specially forged to be the  death  of
 these beasts.
.lm 0
.blank 2
3 Undead
.lm 1 .p
      Once living beings, these souls have become everliving.  But
 in exchange for this "gift" most of them need to drain  the  life
 from the still living.  Beware the touch of a Lich!  Some  of the
 more powerful undead were once mages or evil  priests  and  still
 know their powerful spells.  The best weapon with which to attack
 these foul beings with is a specially blessed to be a Slay Undead
 weapon.  If you have lost some of your life essence to an  undead
 you must obtain a restore life levels potion to regain it.
.lm 0
.blank 2
3 Demons
.lm 1 .p
      These abominations of demonic might  are  quite  powerful.
 Most of them have magical abilities and they can appear  in  just
 about any form of animal or creature. At the lowest levels reside
 the Demon Lords which can summon any of  their  minions  and  the
 Demon, the Balrog, is the ruler of Moria and object of the  quest
 that you have undertaken.  Opposed to  these  creatures  are  the
 various Demon Bane weapons.  With  one of these you  may do  much
 more damage when striking.
.lm 0
.blank 2
3 Evil
.lm 1 .p
      Most of the intelligent creatures in the dungeon  are  evil.
 Of course the Undead and the Demons are evil by  nature,  as well
 most Dragons, but many of the humanoids and monsters are also  of
 an evil nature. Even some of the townspeople are  evil!  The Slay
 Evil weapons and the Holy Avenger were made to defeat these foes,
 but due to the many variations of evil are not as  effective  as,
 say, a Slay Dragon, which is specially attuned. Priest characters
 have many prayers with which to combat Evil monsters  because  of
 their religeous vow to destroy evil.
.lm 0
.blank 2
 
3 Monsters
.lm 1 .p
      Many of the beasts in Moria are only able to survive in  the
 dungeon due to their unnatural nature. These creatures are called
 Monsters and are the foes of the Druid class. Just about anything
 that is not an animal or person is a monster. To fight these foes
 you should get a Slay Monster weapon,  but like Slay Evil,  these
 weapons are not as attuned to the wide variety of creatures.
.lm 0
.blank 2
3 Regenerative
.lm 1 .p
      Some of the monsters, like Trolls and  many  of  the  Demons,
 heal rather rapidly.  If you teleport away because the monster  is
 too dangerous,  you will find that most of the damage you did  has
 healed by the time you meet him again.  A Slay Regenerative weapon
 will do extra damage to these creatures every time that you hit.
.lm 0
.blank 2
3 Mimics
.lm 1 .p
      On occation an object that looks like treasure  may  actually
 be a creature that mimics that shape to fool you into approaching.
 Most can be identified by approaching cautiously, but be careful.
.lm 0
.blank 2
2 Mining
.lm 1 .p
      Much of the treasure within the dungeon can be found only by
 mining  it out of the walls.  Many rich strikes exist within each
 level, but they must  be found and mined.  Quartz  veins are  the
 richest,  yielding the most metals and gems, but magma veins will
 also have some hordes hidden within.
.p
      Mining is very difficult without a  pick or shovel (although
 a strength of  18 or higher  sometimes eliminates  the need for a
 digging tool).  Like many other items, picks and shovels may have
 an additional  magical ability  expressed as  '(+#)'.  The higher
 this number, the more quickly the  tool will enable you to remove
 obstructing quartz, magma, or granite.  A pick or shovel also has
 plusses to hit and damage, and  can be used as a weapon, but most
 other weapons do a  significantly larger amount  of damage, so it
 is always  important to be sure  that you are *NOT* wielding your
 pick/shovel when you are  in combat with a  monster!  Many people
 lose  their characters because  they don't  realize that they are
 still wielding their pick - not their broadsword.
.p
      When a vein of quartz or magma  is  located,  the  character
 should  wield his pick or shovel and begin digging out a section.
 When that section is removed, he should locate another section of
 the  vein,  and  begin  the process again.  Since granite rock is
 much harder to dig through, it is much faster to follow the  vein
 exactly and dig around the granite.
.p
      If the character has a scroll or staff of treasure location,
 he  can  immediately locate all strikes of treasure within a vein
 shown on the screen.  This makes  mining  much  easier  and  more
 profitable.
.p
      Another way to  make mining more  convenient is to wield the
 pick or shovel  as your secondary  weapon, and  then:  use 'x' to
 switch from your attack weapon to your pick (before you dig), dig
 out the  treasure, then  use 'x'  again to  switch back  to  your
 attack weapon (don't  want to get caught fighting with your pick,
 do you?).
.lm 0
.blank 2
.lt
3 Mining_List
.el
.lm 1 .p
.literal
                                  Cost   Weight   Damage  Lvl
 -------------------------------------------------------------
 Orcish Pick                      500     180      1d3     20
 Gnomish Shovel                   100      50      1d2     20
 Dwarven Shovel                   250     120      1d3     40
 Dwarven Pick                    1200     200      1d4     50
.end literal
 
.lm 0
.blank 2
2 Objects
.lm 1 .p
      The dungeon is  full of objects just waiting to be picked up
 and  used.   How  did  they get there?  Well, most  of the useful
 items are  left over  from all the foolish  adventurers  who have
 entered the dungeon before you.  They get killed, and the helpful
 monsters scatter the various  treasures throughout  the  dungeon.
 Most cursed items  have been placed there by evil  sorcerers, who
 enjoy a good joke when it gets you killed.
.p
      You pick up objects by moving on top of them.  You can carry
 as many different items in your backpack as you like, but you are
 restricted by the amount of weight your character can carry. Your
 character's  weight  limit  is  determined by his  Strength.  But
 there are magical items  to increase your  carrying capacity. Any
 items that you put into a Bag of Holding do not count against the
 weight limit. But be careful, poking a hole in the bag could make
 you lose all those items. And never put one Bag into another.
.p
      Only one object may occupy any  one  given  floor  location,
 which may or may not also contain one  monster.  Note that doors,
 traps, and staircases are  considered objects for  this  purpose,
 and that therefore, you cannot drop an object upon a stairwell.
.p
      Many objects found within the dungeon have special  commands
 for their use (see Magic Objects.)  Wands must  be Aimed, scrolls
 must be Read,  potions must  be Quaffed,  and magic items  may be
 Used.  In  any  case, you must first  be able to  carry an object
 before you can use it. Some objects,  such  as chests,  are  very
 complex.  Chests might  contain  other objects  (including  other
 chests) and may be trapped and locked.  Read  the  list of player
 commands  carefully for a further  understanding of  chests  (pay
 special attention to the Search, Disarm and Open commands).
.p
      There is one item in particular that  needs to be explained.
 This is the scroll entitled "Word of Recall".  This scroll can be
 found in the dungeon, and can also be bought in the Temple or Alchemists
  on the
 town level.  It does one of two things, depending on your current
 location.  If you are in  the dungeon,  it teleports  you back to
 town level.  If you are already  on town level, it  teleports you
 to the deepest level  of the dungeon  that you have  already been
 to.  This  makes the  scroll very  useful for returning to deeper
 levels of Moria (who wants to  try and find 30 down stairwells so
 you can get back to level 30 - 1500 feet - by FOOT?).  Unlike all
 other scrolls, this one does NOT act immediately, so don't expect
 it to pull  you out of a combat that  you happen to be losing (it
 usually takes less than 40 turns to act, however).
.p
      One final  warning about objects:  Not  all objects are what
 they seem to be.  Skeletons  lying peacefully  about the dungeons
 have been known to get up and attack....
.lm 0
.blank 2
3 Magic Objects
.lm 1 .p
      There are several types of magical objects that exist to aid
 or harm your adventurer.  Most of them are used  in some  fashion
 but a few must be wielded or  worn to have  an effect.  Of course
 these effects will not always be beneficial to the  character, so
 you may want to identify an item before you don it, but sometimes
 those identify scrolls are hard to come by...
.p
      To use a Chime, Horn or Miscellaneous Item (like a statue or
 a cross) you must (U)se it. To use a Staff you (u)se it, but with
 the lowercase letter.  Wands are (a)imed, Potions (q)uaffed,  and
 Scrolls (r)ead.
.p
 
      Any article of clothing,  armor or jewelry  should be  worn.
 Any weapon or  pick/shovel  should be wielded.  And magic torches
 and lanterns should be held on the equipment inventory. This will
 give you any of the benefits that the item bestows, such as extra
 strength or bonuses to hit and damage.
.p
 
      Gems are a little more complicated.  Most are enchanted with
 a number of uses of some spell.  These are (U)sed.  But a few are
 special gems that float like  Ion Stones about  your helm.  These
 are worn just like armor or jewelry. The best method is to either
 try both methods with it, or identify it to determine if it casts
 spells of adds to your abilities.
.lm 0
.blank 2
.lt
4 Race_Items
.el
.lm 1 .p
      Each of the non-human and  non-halfbreed races  was able  to
 send their best champions into the depths of  Moria with  special
 items that augmented the best aspects of that race. Unfortunately
 no one has succeeded yet and many of these items are lost  in the
 rooms and halls of the dungeon. There is one for each elder race,
 including Orcs, although they  have since  been corrupted  by the
 Balrog's evil. They grant abilities as follows:
.p
      Cloak of Elvenkind - See invisible, Increased stealth, Added
 Charisma, and Sustained Intelligence.
.p
      Girdle of Dwarvenkind - Infravision,  Tunnel ability,  Magic
 resistance, and Sustained Wisdom.
.p
      Hat of Hobbitkind - Infravision, See invisible,  Free Action
 Searching.
.p
      Gloves of the Hive - Added Dexterity, Added Speed.
.p
      Sandels of Dryadkind - Added Charisma, Feather Falling,  See
 invisible, Free Action, Magic resistance.
.p
      Club of Trollkind - 3d4 base weapon damage,  Lose  Charisma,
 Poor Stealth, Poor Searching,  Regeneration,  Resist Acid, Resist
 Cold.
.p
      Ring of Gnomekind - Added Intelligence, Sustain Intelligence
 Slow Digestion.
.p
      Orc Pick (This is a normally unidentifiable enchantment on a
 normal  Orcish  Pick.)  -  Resist  Acid,  Slow  digestion,  Added
 Constitution.
.p
.lm 0
.blank 2
.lt
2 Secret_Doors
.el
.lm 1 .p
      Many secret doors are used within the dungeon to confuse and
 demoralize  adventurers  foolish  enough to enter.  But with some
 luck, and lots of patience, you can find these secret doors.
.p
      Secret doors will  sometimes hide rooms,  corridors, stairs,
 or even entire sections of that  level of the dungeon.  Sometimes
 they simply hide small empty closets or even dead ends.
.p
      Creatures in the dungeon  will generally know of and use all
 secret  doors.   If  they  leave one open, you will be able to go
 right through it.  If they close it behind them you will have  to
 search  for  it  first.  Once  you have discovered a secret door,
 it is  drawn as a  visible door,  and no  more searching  will be
 required in order to use it.
.lm 0
.blank 2
2 Staircases
.lm 1 .p
      Staircases are your primary  means to get to other levels of
 the dungeon.  The symbols for the up  and down staircases are the
 same  as  the  commands  to  use  them.  A  '<' represents  an up
 staircase  and  a '>' represents  a down staircase.  You must put
 your character in the same square with a staircase before you can
 use it.
.p
      Each level has at least one up staircase, and at  least  two
 down  staircases.  There are no exceptions to this rule.  You may
 have trouble finding some  well  hidden  secret  doors,  but  the
 stairs are indeed there.
.p
      In addition to stairwells that go up or down one level, some
 levels have steep stairwells that can move you up or down  by  as many
 four levels.
.p
      The town level has three stairwells, two of which are normal
 down stairwells, and one of which is a steep down stairwell.  You
 will likely want to remember which stairwell is  which,  or  your
 beginning character may find himself in over his head.
.lm 0
.blank 2
2 Weapons
.lm 1 .p
      Carrying a weapon in your backpack does you  no  good.   You
 must  wield a weapon before it can be used in a fight.  Note that
 a secondary weapon can be kept by wielding it and then using  the
 exchange  command  'x'.  A secondary weapon is not in use, simply
 ready to be switched with the current weapon if needed.
.p
      Weapons have two main characteristics;  their ability to hit
 and their ability to do damage, expressed as '(+#,+#)'.  A normal
 weapon would be '(+0,+0)'.  Many weapons in  Moria  have  magical
 bonuses  to  hit  and/or do damage.  Some weapons are cursed, and
 have penalties that hurt the player.   Note  that  cursed  swords
 cannot be un-wielded until the curse is lifted via the use of the
 "Remove Curse" spell.
.p
      Missile  weapons,  such  as  bows,  have  only   one   major
 characteristic, which  is "to hit",  expressed '(+#)'.  This plus
 "to hit" is added to that  of  the missile  used,  if the  proper
 weapon/missiles combination  are used.   (Arrows with Bows, Bolts
 with Crossbows, and Rounded Pebbles or Iron Shots with Slings)
.p
      Although you receive any  magical  bonuses  an  unidentified
 weapon  may  possess when you wield it, those bonuses will not be
 added to the displayed values of "to hit"  and "to dam"  on  your
 character  sheet.   You  must  identify  the  weapon  before  the
 displayed values reflect the real values used.
.p
      Finally, some rare weapons have special abilities.  This are
 called  ego  weapons,  and  are feared by great and meek.  An ego
 sword must be wielded to receive benefit of its abilities.
.p
      Special weapons are denoted by the following abbreviations:
.blank
.lm 15
.list 0 ' '
.le;      (DB) - Demon Bane.
.els
.lm 1
 
          A Demon Bane weapon is a special  purpose  weapon  whose
     sole  intent  is to destroy demon-kind.  Therefore, when used
     against a  demon,  the  amount  of  damage  done  is  greatly
     increased.
.blank
.lm 15
.list 0 ' '
.le;      (DF) - Defender.
.els
.lm 1
 
 
 
          A magical weapon that actually helps the wielder  defend
     himself, thus increasing his armor class.
.blank
.lm 15
.list 0 ' '
.le;      (FB) - Frost Brand.
.els
.lm 1
 
 
          A magical weapon of ice that delivers a cold critical to
     heat based creatures.
.blank
.lm 15
.list 0 ' '
.le;      (FT) - Flame Tongue.
.els
.lm 1
 
 
 
          A magical weapon of flame that delivers a heat  critical
     to cold based creatures.
.blank
.lm 15
.list 0 ' '
.le;      (HA) - Holy Avenger.
.els
.lm 1
 
 
          A Holy Avenger is by far the most powerful  of  weapons.
     Holy  Avengers  have  been  known  to increase several of the
     wielder's stats, to actually  increase  the  wielder's  armor
     class (because of the terror the  weapon spawns in its foes),
     and to actually help the wielder to fight more effectively.
.blank
.lm 15
.list 0 ' '
.le;      (SD) - Slay Dragon.
.els
.lm 1
 
 
          A Slay Dragon weapon is a special purpose  weapon  whose
     sole  intent is to destroy dragon-kind.  Therefore, when used
     against a dragon,  the  amount  of  damage  done  is  greatly
     increased.
.blank
.lm 15
.list 0 ' '
.le;      (SE) - Slay Evil.
.els
.lm 1 .p
 
 
          A Slay Evil weapon is a  special  purpose  weapon  whose
     sole  intent  is  to  destroy  all  forms of evil.  When used
     against an evil creature, either alive or undead, the  damage
     done is greatly increased.
.blank
.lm 15
.list 0 ' '
.le;      (SM) - Slay Monster.
.els
.lm 1
 
 
          A Slay Monster weapon is a special purpose weapon  whose
     sole intent is to destroy all the vile monsters of the world.
     A  monster  is  any  creature  not  natural  to  the   world.
     Therefore  an orc would be a monster, but a giant snake would
     not be.
.blank
.lm 15
.list 0 ' '
.le;      (SR) - Slay Regenerative.
.els
.lm 1 .p
 
 
          A Slay Regenerative weapon is a special  purpose  weapon
     whose  sole  intent is to slay creatures that heal themselves
     through  regeneration.   When  used  against  a  regenerative
     creature, damage done is greatly increased.
.blank
.lm 15
.list 0 ' '
.le;      (SS) - Soul Sword.
.els
.lm 1 .p
 
          Soul Sword weapons are rumored to be  weapons  of  great
     magical   powers,  giving  the  wielder  better  healing  and
     improved vision in addition to the great damage  they do.  It
     is also rumored that the swords take a toll from  their  user
     and can sense the wielder, but worse, few towns  people  like
     wielders of this soul drinking weapon.
.blank
.lm 15
.list 0 ' '
.le;      (SU) - Slay Undead.
.els
.lm 1 .p
 
 
 
     sole  intent  is to destroy all forms of undead.  This weapon
     is hated and feared by the intelligent undead, for  a  single
     blow from this weapon is capable of destroying them.
.blank
.lm 15
.list 0 ' '
.le;      (V) - Vorpal Weapon.
.els
.lm 1 .p
 
 
         A Vorpal Weapon is a magically enhanced weapon  that  has
     the ability to deal a critical blow on occasion.
.blank
.lm 15
.list 0 ' '
.le;      (WB) - Wizard Blade.
.els
.lm 1 .p
 
 
         A Wizard Blade helps the wielder  to  resist  the  spells
     cast by creatures and is a bit lighter  than  its non-magical
     counter-part.  A Mage may use any  Wizard Blade without their
     normal penalty to hit.
.blank
.lm 15
.list 0 ' '
.le;      (BB) - Blessed Blade
.els
.lm 1 .p
 
 
         This weapon has been  specially prepared  so that Priests
     may use  it without the normal penalty.  Due to the religious
     power of the weapon it also will sustain the wielders Wisdom.
 
.lm 0
.blank 2
.lt
3 Weapon_List
.el
.lm 1
.literal
                                  Cost   Weight   Damage Lvl Limit
 -----------------------------------------------------------------
 Bare Hands                         0       0      1d1    0
 Broken dagger                      0      15      1d1    0    M
 Broken sword                       0      75      1d1    0
 Wooden Club                        1     100      1d3    0    C
 Dagger (Stiletto)                 10      12      1d4    0    M
 Dagger (Misercorde)               10      15      1d4    0    M
 Dagger (Bodkin)                   10      20      1d4    1    M
 Dagger (Main Gauche)              25      30      1d5    2    M
 Iron Shod Quarterstaff            25     100      1d5    2    C
 Foil                              35      30      1d5    2    M
 Cat-O-Nine Tails                  14      40      1d4    3    M
 Javelin                           18      30      1d4    4
 Rapier                            42      40      1d6    4    M
 Thrusting Sword (Bilbo)           60      80      1d6    4    M
 Spear                             36      50      1d6    5
 Small Sword                       48      75      1d6    5    M
 Sabre                             50      50      1d7    5
 Thrusting Sword (Baselard)        80     100      1d7    5
 War Hammer                       225     120      3d3    5    C
 Mace                             130     120      2d4    6    C
 Cutlass                           85     110      1d7    7
 Backsword                         60      95      1d9    7
 Awl-Pike                         340     160      1d8    8
 Broadsword                       255     150      2d5    9
 Morningstar                      396     150      2d6   10    C
 Lance                            230     300      2d8   10
 Lucerne Hammer                   376     120      2d5   11
 Longsword                        300     130      1d10  12
 Flail                            353     150      2d6   12    C
 Battle Axe (European)            334     170      3d4   13
 Bastard Sword                    350     140      3d4   14
 Pike                             358     160      2d5   15
 Beaked Axe                       408     180      2d6   15
 Mace (Lead filled)               502     180      3d4   15    C
 Fauchard                         376     170      1d10  17
 Broad Axe                        304     160      2d6   17
 Katana                           400     120      3d4   18
 Ball and Chain                   200     150      2d4   20    C
 Glaive                           363     190      2d6   20
 Halberd                          430     190      3d4   22
 Battle Axe (Balestarius)         500     180      2d8   30
 Two Handed Sword (Claymore)      775     200      3d6   30
 Two Handed Sword (Espadon)       655     180      3d6   35
 Executioner's Sword              850     260      4d5   40
 Two Handed Sword (No-Dachi)      675     200      4d4   45
 Two Handed Great Flail           590     280      3d6   45    C
 Two Handed Sword (Flamberge)    1000     240      4d5   45
 Two Handed Sword (Zweihander)   1000     280      4d6   50
 
 Short Bow                         50      30      0d0      3
 Long Bow                         120      40      0d0     10
 Light Crossbow                   140     110      0d0     15
 Heavy Crossbow                   300     200      1d1     30
 Composite Bow                    240      40      0d0     40
 
 Rounded Pebble                     1       4      1d2      0
 Sling                              5       5      0d0      1
 Iron Spike                         1      10      1d1      1
 Arrow                              1       2      1d4      2
 Bolt                               2       3      1d5      2
 Iron Shot                          2       5      1d3      3
.end literal
.lm 0
.blank 2
2 Winning
.lm 1 .p
      Once your character  has progressed  to killing dragons with
 but  a  mean  glance  and snap of his fingers, he MAY be ready to
 take on the Balrog.  The Balrog will appear on every level  after
 level 99 (4950 feet), so don't go down there  until you are ready
 for him.
.p
      The Balrog cannot be killed in some of  the  easier  methods
 used  on  normal  creatures.  The Balrog is immune to genocide,
 polymorph, sleep, and
 charm.  Magical spells like coldball are effective against him,
 as  are  weapons,  but he is difficult to kill and, if allowed to
 escape for a time, can heal himself.
.p
      If you should actually survive the attempt  of  killing  the
 Balrog,  you  will recieve  the status of *WINNER*.  Since you have
 defeated the toughest monster alive, your character is  ready  to
 retire  and  cannot  be  saved.   When  you  quit  the game, your
 character recieves a surprise bonus score and may entered into the
 high score list (depending on points accumulated).
.lm 0
.blank 2
2 Wizards
.lm 1 .p
      There are rumors of Moria Wizards who, if asked  nicely, can
 explain  details  of the  Moria  game that may  seem difficult to
 beginners.  In addition,  they  have  special  spells  which  can
 restore characters  lost by  accident,  such as  power-outs, etc.
 Send a  mail  message to "VAX1::JAVERT"  if you need  to contact  one of
 these wizards.
.lm 0
.blank 2
1 Character
.lm 1 .p
      All characters have six main attributes which  modify  their
 basic   abilities.   These  six  attributes,  called  stats,  are
 STRENGTH,  INTELLIGENCE,  WISDOM,  DEXTERITY,  CONSTITUTION,  and
 CHARISMA.   Stats may  vary  from a  minimum of 3 to a maximum of
 18/100.   Because  adventurers  tend  to  be better  than average
 characters,  Moria stats will  average  about  12.5, and  will be
 further  adjusted  by  race  and  class.   Some  races  are  just
 naturally  better  at  being  certain  classes,  as will be shown
 later.
.p
      In addition to the more visible stats,  each  character  has
 certain  abilities  which  are  mainly determined by his race and
 class, but are also modified by his  stats.   The  abilities  are
 FIGHTING,   THROWING/BOWS,   SAVING  THROW,  STEALTH,  DISARMING,
 MAGICAL DEVICES, PERCEPTION, SEARCHING,  INFRA-VISION,  SWIMMING,
 and REPUTATION.
.p
      Characters will be assigned an early history, with money and
 a social class based on that history.  Starting money is assigned
 based on social class, charisma, and somewhat  upon the  average  of a
 character's  stats.   A  character  with below average stats will
 receive extra money to help him survive the first adventure.
.p
     Each character will  also have  physical attributes such as a
 height, weight, sex, and a physical  description.  None of these,
 except weight, play any part in the game  other than  to give the
 player  a  "feeling"  for  his  character.   Weight  is  used for
 computing carrying capacity and also for BASHING.
.p
     Finally each character  is assigned HIT POINTS based on their
 race,  class,  and constitution.  Spell casters will also receive
 MANA which  is expended when  casting spells.  MANA is  based  on
 Wisdom   for  Priests,  Intelligence  for   Mages,  Charisma  and
 Dexterity  for Bards, and Charisma and Wisdom for Druids.
.lm 0
.blank 2
2 Abilities
.lm 1 .p
      Characters possess nine different abilities which  can  help
 them to survive.  The starting abilities of a character are based
 upon its race and  class.  Abilities  may be adjusted  by high or
 low stats, and may increase with the level of the character.
.lm 0
.blank 2
3 Fighting
.lm 1 .p
      Fighting is the ability to hit and do damage with weapons or
 fists.   Normally a character gets a single blow from any weapon,
 but if his dexterity and strength are high enough, he may receive
 more   blows  per  round  with  lighter  weapons.   Strength  and
 dexterity both  modify  the ability  to hit  an  opponent.   This
 skill increases with the level of the character. Monks  get  more
 attacks with their hands as they increase their levels  and  have
 the best chance to obtain a critical hit when attacking.  Many of
 the characters have some restrictions on the  weapons  that  they
 may use  freely.  Mages are very restricted in that they may only
 use small weapons like daggers. Clerics are also restricted since
 they must avoid shedding blood.  Therefore they use blunt weapons
 like maces and hammers (um well, less blood).
 Druids may use both of the types that are
 allowed to Mages and Clerics and Bards  may  use  any  dagger  or
 sword freely.  Rogues may use anything except polearms  and  the  other
 classes;  Adventurer, Ranger, Paladin, Monk, and Warrior; may use
 any weapon without a penelty. If a character wields a weapon that
 is not commonly used by that class then he will suffer a  penalty
 to hit due to his poor training.
.lm 0
.blank 2
3 Throwing/Bows
.lm 1 .p
      This skill  includes  the use  of  missile  weapons, and the
 throwing of objects.  Different stats apply  for certain weapons,
 and may modify the distance an object is thrown/fired, the amount
 of damage  done, and the  ability to hit a  creature.  This skill
 also increases with the level of the character. Any character may
 use a bow, sling or crossbow freely.
.lm 0
.blank 2
.lt
3 Saving_Throw
.el
.lm 1 .p
      A Saving Throw is the ability of a character to  resist  the
 effects  of a spell cast on him by another person/creature.  Note
 that this does not include spells cast on the player by  his  own
 stupidity, such as by quaffing  a  nasty  potion.   This  ability
 increases with the level of the character,  but most  high  level
 creatures are better at casting spells, so it tends to even out.
.lm 0
.blank 2
3 Stealth
.lm 1 .p
      The  ability to  move silently is  very  useful.  Characters
 with good stealth can  usually surprise  their opponents, gaining
 the first blow.  Also, creatures may  fail  to notice  a stealthy
 character entirely, allowing a player to avoid certain fights.
.lm 0
.blank 2
3 Disarming
.lm 1 .p
      Disarming is the  ability  to  remove  traps  (safely),  and
 includes   picking  locks  on  traps  and  doors.   Note  that  a
 successful disarming will gain  the  character  some  experience.
 The  character  must  have found  a trap  (possibly by searching)
 before it  can  be  disarmed.  Dexterity  and  intelligence  both
 modify the ability to disarm, and this ability increases with the
 level of the character.
.lm 0
.blank 2
.lt
3 Using_Magical_Devices
.el
.lm 1 .p
      Using a magical device such as  a  wand  or  staff  requires
 experience  and knowledge.  Spell users such as mages and priests
 are therefore  much better at using a magical device than, say, a
 fighter.   This  skill is modified by intelligence, and increases
 with the level of the character.
.lm 0
.blank 2
3 Perception
.lm 1 .p
      Perception  is  the  ability  to  notice  something  without
 actively  seeking  it out.  More  specifically, it is the ability
 to find secret doors and traps without  searching for them.  This
 skill is  solely determined  by race and  class, and  will  never
 improve unless magically enhanced.
.lm 0
.blank 2
3 Searching
.lm 1 .p
      To search is to actively look for secret doors, floor traps,
 and  traps  on  chests.   Rogues  are  the best at searching, but
 monks, mages, rangers, and priests are good at  it.  Intelligence
 modifies  your  searching  ability,  and your  ability will  also
 increase with your level.
.lm 0
.blank 2
3 Infra-Vision
.lm 1 .p
      Infra-vision is the ability to see heat sources.  Since most
 of  the  dungeon is cool or cold, infra-vision will not allow the
 player to see walls  and  objects.   Infra-vision  will  allow  a
 character  to  see  any  warm-blooded  creature  up to  a certain
 distance.  This ability works equally well with  or  with  out  a
 light  source.   Note  that the majority of Moria's creatures are
 cold blooded, and will not be detected unless lit up by  a  light
 source.
.lm 0
.blank 2
3 Swimming
.lm 1 .p
      The ability to  swim  can  greatly  aid  your  character  in
 running  away from monsters that swim poorly.  Also, it will help
 you to avoid those creatures that are native to the water.
.lm 0
.blank 2
3 Reputation
.lm 1 .p
      Your reputation is a measure of  how  well  the  townspeople
 react  with  you.   For  every  innocent  creature you kill, your
 reputation is  lowered.   Be  warned  that  if  you  kill  enough
 innocent creatures, the townspeople will consider you a menace to
 society, and takes steps to remove you from it.
.lm 0
.blank 2
2 Classes
.lm 1 .p
      Once a race has been chosen, you will need to pick a  class.
 Some  classes  will  not  be  available  to  certain  races;  for
 instance, a Half-Troll  cannot  become a  Paladin.  For the first
 few adventures it is suggested  that you  run a warrior, monk, or
 rogue. Spell casting classes tend to require much more experience
 with the game to keep them alive.
.p
      The section entitled Experience  gives the  number of points
 needed to reach each experience level for a given class.
.lm 0
.blank 2
3 Warrior
.lm 1 .p
      A Warrior is a hack-and-slash character, who solves most  of
 his  problems  by  cutting  them to pieces, but will occasionally
 fall back on the help of a magical device.  His prime  stats  are
 Strength  and  Constitution, and a good Dexterity can really help
 at times.  A Warrior will be good at Fighting and  Throwing/Bows,
 but bad at most other skills. The main advantage for the  Warrior
 is his high number of hit points and his great strength  as  well
 as the number of attacks he can do at higher levels.
.lm 0
.blank 2
3 Mage
.lm 1 .p
      A Mage must live by his wits.  He cannot hope to simply hack
 his  way through the dungeon, and so must therefore use his magic
 to defeat, deceive, confuse, and escape the monsters.  A mage  is
 not really complete without a backpack full of magical devices to use
 in  addition  to his  spells.  He  can  master  the  higher level
 magical devices far  easier  than  anyone  else, and has the best
 saving throw to  resist effects of spells cast at him.  His prime
 stats are Intelligence and Dexterity.  There is no rule that says
 that a mage cannot become a good fighter, but spells are his true
 realm.
.lm 0
.blank 2
3 Priest
.lm 1 .p
      A Priest is a character of holy devotion.  They explore  the
 dungeon  only  to  destroy  the  evil  that  lurks within, and if
 treasure just happens to fall  into their packs,  well,  so  much
 more  to the glory of their church!  Priests receive their spells
 from a deity, and therefore do not choose which spells they  will
 learn.   He  is familiar with magical devices, preferring to call
 them instruments of god, but is not as good as a  mage  in  their
 use.   Priests have good saving throws, and make decent fighters,
 preferring blunt weapons over edged ones.   Wisdom  and  Charisma
 are the priest's primary stats.
.lm 0
.blank 2
3 Rogue
.lm 1 .p
      A Rogue is a character that prefers to live by his  cunning,
 but  is  capable  of fighting his way out of a tight spot.  He is
 the master of traps and locks, no device being impossible for him
 to  overcome.   A rogue has a high stealth, allowing him to sneak
 around many creatures without having to fight, or to sneak up and
 get  the  first  blow.   A  rogue's perception is higher than any
 other class, and many times he will notice a trap or secret  door
 before  having  to  search.   A  rogue is better than warriors or
 paladins with magical devices, and can gain  a significant number
 of  bard spells, but cannot rely on their performance.  A rogue's
 primary stats are Dexterity and Charisma.
.lm 0
.blank 2
3 Ranger
.lm 1 .p
      A Ranger is a druid/fighter. He is a good fighter,  and  the
 best  of  the  classes  with a missile weapon such as a bow.  The
 ranger learns songs much more slowly than a druid, but is capable
 of  learning all but the most powerful song.  Because a ranger is
 really a dual class character, it  requires  more  experience  to
 advance  him.  A ranger has a good stealth, good perception, good
 searching, a good saving throw, and is good with magical devices.
 His primary stats are Charisma and Strength.
.lm 0
.blank 2
3 Paladin
.lm 1 .p
      A Paladin is a fighter/priest.  He is a very  good  fighter,
 second  only  to  the warrior class, but not very good at missile
 weapons.  He receives prayers at a slower pace  then a priest,
 and   can  receive  all  but the most powerful prayer.  Because a
 paladin is really  a  dual  class  character,  it  requires  more
 experience  to  advance  him.  A paladin lacks much in the way of
 abilities.  He is poor at  stealth,  perception,  searching,  and
 magical  devices.  He has a decent saving throw due to his divine
 alliance.  His primary stats are Strength and Wisdom.
.lm 0
.blank 2
3 Bard
.lm 1 .p
     A Bard is a charmer  and master of lore.  They  have  a  wide
 selection of informational and  monster  affecting  spells.  They
 have  good knowledge of magic items and have some of the  stealth
 and  fighting ablilities of the rogue class but  depend  more on
 their songs' ablilities to put monsters off guard  or  his  refined
 skill with  the  bow.  Charisma and Dexterity are the Bard's  main
 characteristics.
.lm 0
.blank 2
3 Druid
.lm 1 .p
     Druids live in the woods and deal with nature, but their hatred
 of monsters draws them down into the vaults of Moria  to  destroy
 these horrible creatures. Druids are second in magical power only
 to Mages, but this class has more protective magic as well. Druid
 characters learn their nature based music at random  but,  unlike
 Mages, have some fighting ability.  The primary stats for a Druid
 are Charisma and Wisdom.
.lm 0
.blank 2
3 Adventurer
.lm 1 .p
      This is a special character that combines the  abilities  of
 the mage with some fighting and thieving  abilities.  They can do
 almost anything better than a mage, but do not learn or cast  the
 spells as well as the specialized magician and can not match most
 of the other classes at any one task.  
 The primary  stats for an Adventurer are
 Strength and Intelligence.
.lm 0
.blank 2
3 Monk
.lm 1 .p
 The Monk's studies in monestaries give him  a  broader  range  of
 skills than the fighter and Monks have the ability to strike with
 their hands for significant damage.  But Monks do  not  have  the
 same hardiness as the pure warrior and they have less 
 skill in archery. Lastly, the Monk has a better chance of  making
 a critical blow against his foes than others as he gains  levels.
 Dexterity and Strength are the prime stats for a Monk.
.lm 0
.blank 2
3 Experience
.lm 1 .p
      Please type the number listed under the character-class/race
 combination that you wish to know the experience levels for.
.blank 2
.lm 0
.literal
               Warrior Monk  Priest Paladin Druid Ranger
 -------------+------+------+------+------+------+------+
 | Human      | 1.00 | 1.10 | 1.10 | 1.35 | 1.20 | 1.30 |
 | Half-Elf   | 1.10 | 1.20 | 1.20 | 1.45 | 1.30 | 1.40 |
 | Elf        | 1.20 |  --  | 1.30 |  --  | 1.40 | 1.50 |
 | Halfling   |  --  | 1.20 |  --  |  --  | 1.30 |  --  |
 | Gnome      |  --  |  --  | 1.35 |  --  | 1.45 |  --  |
 | Dwarf      | 1.20 |  --  | 1.30 |  --  | 1.40 |  --  |
 | Half-Orc   | 1.10 | 1.20 | 1.20 |  --  |  --  |  --  |
 | Half-Troll | 1.20 |  --  | 1.30 |  --  |  --  |  --  |
 | Phraint    | 1.20 | 1.30 |  --  |  --  |  --  | 1.50 |
 | Dryad      |  --  | 1.30 | 1.30 |  --  | 1.40 | 1.50 |
 -------------+------+------+------+------+------+------+

 
                        Mage  Adven.  Bard   Rogue
         -------------+------+------+------+------+
         | Human      | 1.30 | 1.40 | 1.30 | 1.10 |
         | Half-Elf   | 1.40 | 1.50 | 1.40 | 1.20 |
         | Elf        | 1.50 | 1.60 | 1.50 | 1.30 |
         | Halfling   |  --  | 1.40 | 1.40 | 1.20 |
         | Gnome      | 1.55 |  --  |  --  | 1.35 |
         | Dwarf      |  --  |  --  |  --  |  --  |
         | Half-Orc   |  --  |  --  |  --  | 1.20 |
         | Half-Troll |  --  |  --  |  --  |  --  |
         | Phraint    | 1.50 | 1.50 | 1.50 | 1.30 |
         | Dryad      |  --  |  --  | 1.50 |  --  |
         -------------+------+------+------+------+
.end literal
4 1.00
.blank 0
.lm 0
.literal
  1st :     0           15th :    1400          29th :    50000
  2nd :    10           16th :    1800          30th :    75000
  3rd :    20           17th :    2300          31st :   100000
  4th :    45           18th :    2900          32nd :   150000
  5th :    70           19th :    3600          33rd :   200000
  6th :   100           20th :    4400          34th :   300000
  7th :   140           21st :    5400          35th :   400000
  8th :   200           22nd :    6800          36th :   500000
  9th :   280           23rd :    8400          37th :   750000
 10th :   380           24th :   10200          38th :  1500000
 11th :   500           25th :   12500          39th :  2500000
 12th :   650           26th :   17500          40th :  5000000
 13th :   850           27th :   25000
 14th :  1100           28th :   35000
.end literal
4 1.10
.blank 0
.lm 0
.literal
  1st :     0           15th :    1540          29th :    55000
  2nd :    11           16th :    1980          30th :    82500
  3rd :    27           17th :    2530          31st :   110000
  4th :    49           18th :    3190          32nd :   165000
  5th :    77           19th :    3960          33rd :   220000
  6th :   110           20th :    4840          34th :   330000
  7th :   154           21st :    5940          35th :   440000
  8th :   220           22nd :    7480          36th :   550000
  9th :   308           23rd :    9240          37th :   825000
 10th :   418           24th :   11220          38th :  1650000
 11th :   550           25th :   13750          39th :  2750000
 12th :   715           26th :   19250          40th :  5500000
 13th :   935           27th :   27500
 14th :  1210           28th :   38500
.end literal
4 1.20
.blank 0
.lm 0
.literal
  1st :     0           15th :    1680          29th :    60000
  2nd :    12           16th :    2160          30th :    90000
  3rd :    30           17th :    2760          31st :   120000
  4th :    54           18th :    3480          32nd :   180000
  5th :    84           19th :    4320          33rd :   240000
  6th :   120           20th :    5280          34th :   360000
  7th :   168           21st :    6480          35th :   480000
  8th :   240           22nd :    8160          36th :   600000
  9th :   336           23rd :   10080          37th :   900000
 10th :   456           24th :   12240          38th :  1800000
 11th :   600           25th :   15000          39th :  3000000
 12th :   780           26th :   21000          40th :  6000000
 13th :  1020           27th :   30000
 14th :  1320           28th :   42000
.end literal 
4 1.25
.blank 0
.lm 0
.literal
  1st :     0           15th :    1750          29th :    62500
  2nd :    12           16th :    2250          30th :    93750
  3rd :    31           17th :    2875          31st :   125000
  4th :    56           18th :    3625          32nd :   187500
  5th :    87           19th :    4500          33rd :   250000
  6th :   125           20th :    5500          34th :   375000
  7th :   175           21st :    6750          35th :   500000
  8th :   250           22nd :    8500          36th :   625000
  9th :   350           23rd :   10500          37th :   937500
 10th :   475           24th :   12750          38th :  1875000
 11th :   625           25th :   15625          39th :  3125000
 12th :   812           26th :   21875          40th :  6250000
 13th :  1062           27th :   31250
 14th :  1375           28th :   43750
.end literal
4 1.30
.blank 0
.lm 0
.literal
  1st :     0           15th :    1820          29th :    65000
  2nd :    13           16th :    2340          30th :    97500
  3rd :    32           17th :    2990          31st :   130000
  4th :    58           18th :    3770          32nd :   195000
  5th :    91           19th :    4680          33rd :   260000
  6th :   130           20th :    5720          34th :   390000
  7th :   182           21st :    7020          35th :   520000
  8th :   260           22nd :    8840          36th :   650000
  9th :   364           23rd :   10920          37th :   975000
 10th :   494           24th :   13260          38th :  1950000
 11th :   650           25th :   16250          39th :  3250000
 12th :   845           26th :   22750          40th :  6500000
 13th :  1105           27th :   32500
 14th :  1430           28th :   45500
.end literal 
4 1.35
.blank 0
.lm 0
.literal
  1st :     0           15th :    1890          29th :    67500
  2nd :    13           16th :    2430          30th :   101250
  3rd :    33           17th :    3105          31st :   135000
  4th :    60           18th :    3915          32nd :   202500
  5th :    94           19th :    4860          33rd :   270000
  6th :   135           20th :    5940          34th :   405000
  7th :   189           21st :    7290          35th :   540000
  8th :   270           22nd :    9180          36th :   675000
  9th :   378           23rd :   11340          37th :  1012500
 10th :   513           24th :   13770          38th :  2025000
 11th :   675           25th :   16875          39th :  3375000
 12th :   877           26th :   23625          40th :  6750000
 13th :  1147           27th :   33750
 14th :  1485           28th :   47250
.end literal 
4 1.40
.blank 0
.lm 0
.literal
  1st :     0           15th :    1960          29th :    70000
  2nd :    14           16th :    2520          30th :   105000
  3rd :    35           17th :    3220          31st :   140000
  4th :    63           18th :    4060          32nd :   210000
  5th :    98           19th :    5040          33rd :   280000
  6th :   140           20th :    6160          34th :   420000
  7th :   196           21st :    7560          35th :   560000
  8th :   280           22nd :    9520          36th :   700000
  9th :   392           23rd :   11760          37th :  1050000
 10th :   532           24th :   14280          38th :  2100000
 11th :   700           25th :   17500          39th :  3500000
 12th :   910           26th :   24500          40th :  7000000
 13th :  1190           27th :   35000
 14th :  1540           28th :   49000
.end literal 
4 1.45
.blank 0
.lm 0
.literal
  1st :     0           15th :    2030          29th :    72500
  2nd :    14           16th :    2610          30th :   108750
  3rd :    36           17th :    3335          31st :   145000
  4th :    65           18th :    4205          32nd :   217500
  5th :   101           19th :    5220          33rd :   290000
  6th :   145           20th :    6380          34th :   435000
  7th :   203           21st :    7830          35th :   580000
  8th :   290           22nd :    9860          36th :   725000
  9th :   406           23rd :   12180          37th :  1087500
 10th :   551           24th :   14790          38th :  2175000
 11th :   725           25th :   18125          39th :  3625000
 12th :   942           26th :   25375          40th :  7250000
 13th :  1232           27th :   36250
 14th :  1595           28th :   50750
.end literal 
4 1.50
.blank 0
.lm 0
.literal
  1st :     0           15th :    2100          29th :    75000
  2nd :    15           16th :    2700          30th :   112500
  3rd :    37           17th :    3450          31st :   150000
  4th :    67           18th :    4350          32nd :   225000
  5th :   105           19th :    5400          33rd :   300000
  6th :   150           20th :    6600          34th :   450000
  7th :   210           21st :    8100          35th :   600000
  8th :   300           22nd :   10200          36th :   750000
  9th :   420           23rd :   12600          37th :  1125000
 10th :   570           24th :   15300          38th :  2250000
 11th :   750           25th :   18750          39th :  3750000
 12th :   975           26th :   26250          40th :  7500000
 13th :  1275           27th :   37500
 14th :  1650           28th :   52500
.end literal 
4 1.55
.blank 0
.lm 0
.literal
  1st :     0           15th :    2170          29th :    77500
  2nd :    15           16th :    2790          30th :   116250
  3rd :    38           17th :    3565          31st :   155000
  4th :    69           18th :    4495          32nd :   232500
  5th :   108           19th :    5580          33rd :   310000
  6th :   155           20th :    6820          34th :   465000
  7th :   217           21st :    8370          35th :   620000
  8th :   310           22nd :   10540          36th :   775000
  9th :   434           23rd :   13020          37th :  1162500
 10th :   589           24th :   15810          38th :  2325000
 11th :   775           25th :   19375          39th :  3875000
 12th :  1007           26th :   27125          40th :  7750000
 13th :  1317           27th :   38750
 14th :  1705           28th :   54250
.end literal 
4 1.60
.blank 0
.lm 0
.literal
  1st :     0           15th :    2240          29th :    80000
  2nd :    16           16th :    2880          30th :   120000
  3rd :    40           17th :    3680          31st :   160000
  4th :    72           18th :    4640          32nd :   240000
  5th :   112           19th :    5760          33rd :   320000
  6th :   160           20th :    7040          34th :   480000
  7th :   224           21st :    8640          35th :   640000
  8th :   320           22nd :   10880          36th :   800000
  9th :   448           23rd :   13440          37th :  1200000
 10th :   608           24th :   16320          38th :  2400000
 11th :   800           25th :   20000          39th :  4000000
 12th :  1040           26th :   28000          40th :  8000000
 13th :  1360           27th :   40000
 14th :  1760           28th :   56000
.el
.lm 0 .blank 2
2 Races
.lm 1 .p
      there are eight different races that you can choose from  in
 Moria.   Some races are restricted as to what profession they may
 be, and each race  has  its  own  adjustments  to  a  character's
 abilities.
.lm 0 .blank 2
3 Human
.lm 1 .p
      The human  is  the  base  character,  all  other  races  are
 compared  to  him.  Humans can  choose any class, and are average
 at everything.  Humans tend to go up levels faster than any other
 race, because of their shorter life-spans.  No racial adjustments
 occur to characters choosing human.
.lm 0 .blank 2
3 Half-Elf
.lm 1 .p
      Half-elves tend  to be smarter and  faster than humans,  but
 not  as  strong.   Half-elves  are  slightly better at searching,
 disarming, perception, stealth, and magic, but they  are  not  as
 good at hand weapons.  Half-elves may choose any class.
.lm 0 .blank 2
3 Elf
.lm 1 .p
      Elves are better magicians then humans, but not as  good  at
 fighting.   They tend to be smarter and faster than either humans
 or half-elves, and also have better wisdom.  Elves are better  at
 searching,  disarming,  perception,  stealth, and magic, but they
 are not as good at hand weapons.   Elves  may  choose  any  class
 except Paladin.
.lm 0 .blank 2
3 Halfling
.lm 1 .p
      Halflings, or Hobbits, are very good at bows, throwing,  and
 have  good  saving throws.  They also are very good at searching,
 disarming, perception,  and  stealth,   so  they  make  excellent
 thieves (but prefer to be called burglars...).  They will be much
 weaker than humans, and not good at bashing. Halflings have  fair
 infra-vision,  so  can  detect  warm  creatures  at  a  distance.
 Halflings can choose  between being a monk, mage, druid,  bard or
 rogue.
.lm 0 .blank 2
3 Gnome
.lm 1 .p
      Gnomes are smaller than dwarfs, but larger  than  halflings.
 They, like the halflings, live in the earth in burrow-like homes.
 Gnomes are practical jokers, so if they can kill something  in  a
 humorous  way,  so much the better.  Gnomes make excellent mages,
 and have very good saving throws.  They are  good  at  searching,
 disarming,  perception,  and  stealth.  They have lower strength
 and constitution than humans,  therefore do not make  as  good
 of  fighters.   Gnomes have fair infra-vision, so can detect warm
 creatures at a distance.  A gnome  may  choose  between  being  a
 druid, mage, priest, or rogue.
.lm 0 .blank 2
3 Dwarf
.lm 1 .p
      Dwarves are the headstrong miners and  fighters  of  legend.
 Dwarves make excellent choices for a warrior or priest.  Dwarves  tend  to  be
 stronger and have  higher constitutions, but  are slower and less
 intelligent than humans.  Because they are so headstrong and  are
 somewhat  wise,  they  resist spells  which  are  cast upon them.
 Dwarves  also  have   good   infra-vision   because   they   live
 underground.  They do have one big  drawback though.  Dwarves are
 loud-mouthed and proud, singing  in  loud  voices,  arguing  with
 themselves  for  no  good  reason,  screaming  out  challenges at
 imagined foes.  In other words, dwarves have a miserable stealth.
.lm 0 .blank 2
3 Half-Orc
.lm 1 .p
      Half-Orcs make excellent fighters, and decent  priests,  but
 are  terrible  at  magic.  They are as bad as dwarves at stealth,
 and horrible at searching, disarming, and perception.   Half-Orcs
 are,   let's  face  it, ugly.  They tend to pay more for goods in
 town.  Half-Orcs do make  good priests and rogues, for the simple
 reason  that  Half-Orcs tend to have great constitutions and lots
 of hit points.
.lm 0 .blank 2
3 Half-Troll
.lm 1 .p
      Half-Trolls are incredibly strong, and have the highest  hit
 points  of  any  character  race.   They are also very stupid and
 slow.  They will make great fighters and iffy priests.  They  are
 bad  at  searching, disarming, perception, and stealth.  They are
 so ugly that a Half-Orc grimaces in their  presence.   They  also
 happen to be fun to run....
.lm 0 .blank 2
3 Phraint
.lm 1 .p
      Phraints are giant mantis-like insects, who  live  in  hive-like
 societies.  They have incredibly high dexterity, which gives
 them very good stealth,  but their insect appearance  gives  them
 low charisma.  They are average or below in most other statistics
 and skills. Due to their culture they make good monks and rogues,
 but can also become mages, bards, rangers and adventurers.
.lm 0 .blank 2
3 Dryad
.lm 1 .p
      This race is born of noble trees but for some reason is only
 female. Their beauty is unmatched and they are quite wise, due to
 their longevity. They are raised with Elves and have similar
 skills. Due to their  nature  they  make the  best druids but are
 not suited to be mages. They may also  become  priests, monks  or
 bards, but do not have the temperment to become warriors.
.lm 0 .blank 2
2 Sex
.lm 1 .p
      You may choose to be either a male or  a  female  character.
 Only height and weight are affected by a character's sex.  Female
 characters tend to be somewhat smaller  and  lighter  then  their
 male counterparts.  No adjustments to stats or abilities are made
 because of the sex of a character.
.lm 0 .blank 2
 
2 Stats
.lm 1 .p
      Characters have 6 primary  stats that  describe them.  These
 stats  are  called  Strength,  Intelligence,  Wisdom,  Dexterity,
 Constitution, and Charisma.
.p
      The  average  stats  for  each  legal   character-class/race
 combination are given under Average Stats.
.lm 0 .blank 2
3 Strength
.lm 1 .p
      Strength is important in fighting with weapons, or  hand  to
 hand.   A  high strength can improve your chances of hitting, and
 the amount of damage done with each  hit.   Characters  with  low
 strengths  may  receive  penalties.   Strength  is also useful in
 tunneling, body and shield bashing, etc.
.lm 0 .blank 2
3 Intelligence
.lm 1 .p
      Intelligence is the prime stat of a mage,  or  magician.   A
 high  intelligence  increases a mages chances of learning spells,
 and in gaining mana.  No spell  may  be  learned  by  mages  with
 intelligence below eight.  Intelligence also modifies a character's
 chance of disarming traps and picking locks.
.lm 0 .blank 2
3 Wisdom
.lm 1 .p
      Wisdom is the  prime  stat  of  a  priest.   A  high  wisdom
 increases  the  chance  of  receiving  new spells from a priest's
 deity, and in the  gaining  of  mana.   Wisdom  also  modifies  a
 character's  chance  of  resisting  magical  spells cast upon his
 person.
.lm 0 .blank 2
3 Dexterity
.lm 1 .p
      Dexterity is the combination of agility  and  quickness.   A
 high  dexterity  may allow a character to get multiple blows with
 lighter weapons, thus greatly increasing his kill power, and  may
 increase  his  chances  of hitting with any weapon.  Dexterity is
 also useful in picking locks and disarming traps.
.lm 0 .blank 2
3 Constitution
.lm 1 .p
      Constitution is a character's ability to  resist  damage  to
 their  body,  and  to  recover from damage received.  Therefore a
 character with a high constitution will receive more hit  points,
 and be more resistant to poisons.
.lm 0 .blank 2
3 Charisma
.lm 1 .p
      Charisma represents a character's personality,  as  well  as
 physical  looks.   A  character with a high charisma will receive
 better prices from store owners, whereas a character with a  very
 low  charisma  will  be  robbed blind.  A high charisma will also
 mean more money when the character starts out. Bards  and  Druids
 may also learn more songs if they have  high  charisma (although,
 this chance is also modified by  dexterity  for Bards and  wisdom
 for Druids.)
.lm 0 .blank 2
3 Average_Stats
.lm 1 .p
      Select the character class you wish the list of stats for.
.lm 0 .blank 2
4 Warrior
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHA
 -------------+-----+-----+-----+-----+-----+-----+
 | Human      |  17 |  10 |  10 |  14 |  14 |  11 |
 | Half-Elf   |  16 |  11 |  10 |  15 |  13 |  12 |
 | Elf        |  16 |  12 |  11 |  15 |  12 |  12 |
 | Dwarf      |18/10|   7 |  11 |  12 |  16 |   8 |
 | Half-Orc   |18/10|   9 |  10 |  14 |  15 |   7 |
 | Half-Troll |18/30|   6 |   8 |  10 |  17 |   5 |
 | Phraint    |  17 |  10 |   6 |18/20|  14 |   8 |
 -------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
4 Mage
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHAp
 -------------+-----+-----+-----+-----+-----+-----+
 | Human      |   7 |  15 |  12 |  12 |  10 |  12 |
 | Half-Elf   |   6 |  16 |  12 |  13 |   9 |  13 |
 | Elf        |   6 |  17 |  13 |  13 |   8 |  13 |
 | Halfling   |   5 |  17 |  13 |  15 |  11 |  13 |
 | Gnome      |   6 |  17 |  12 |  14 |  11 |  10 |
 | Phraint    |   7 |  15 |   8 |  18 |  10 |   9 |
 -------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
 
4 Priest
.lm 1
.literal
 
                STR   INT   WIS   DEX   CON   CHA
 -------------+-----+-----+-----+-----+-----+-----+
 | Human      |  12 |   9 |  15 |  11 |  13 |  14 |
 | Half-Elf   |  11 |  10 |  15 |  12 |  12 |  15 |
 | Elf        |  11 |  11 |  16 |  12 |  11 |  15 |
 | Gnome      |  11 |  11 |  15 |  13 |  12 |  12 |
 | Dwarf      |  14 |   6 |  16 |  11 |  14 |  11 |
 | Half-Orc   |  14 |   8 |  15 |  11 |  14 |  10 |
 | Half-Troll |  16 |   5 |  11 |   7 |  16 |   8 |
 | Dryad      |  11 |   9 |  18 |  11 |  11 |  17 |
 -------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
 
4 Rogue
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHA
 -------------+-----+-----+-----+-----+-----+-----+
 | Human      |  14 |  12 |  10 |  15 |  13 |  13 |
 | Half-Elf   |  13 |  13 |  10 |  16 |  12 |  14 |
 | Elf        |  13 |  14 |  11 |  16 |  11 |  14 |
 | Halfling   |  12 |  14 |  11 |  18 |  14 |  14 |
 | Gnome      |  13 |  14 |  10 |  17 |  14 |  11 |
 | Half-Orc   |  16 |  11 |  10 |  15 |  14 |   9 |
 | Phraint    |  14 |  13 |   6 |18/30|  13 |  10 |
 -------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
 
4 Ranger
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHA
 -------------+-----+-----+-----+-----+-----+-----+
 | Human      |  14 |  12 |  13 |  13 |  13 |  14 |
 | Half-Elf   |  13 |  13 |  13 |  14 |  12 |  15 |
 | Elf        |  13 |  14 |  14 |  14 |  11 |  15 |
 | Dryad      |  13 |  12 |
 -------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
 
4 Paladin
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHA
 -------------+-----+-----+-----+-----+-----+-----+
 | Human      |  15 |   9 |  13 |  12 |  14 |  14 |
 | Half-Elf   |  14 |  10 |  13 |  13 |  13 |  15 |
 -------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
 
4 Druids
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHA
 -------------+-----+-----+-----+-----+-----+-----+
 | Human      |  11 |  11 |  14 |  12 |  12 |  15 |
 | Half-Elf   |  10 |  12 |  14 |  13 |  11 |  16 |
 | Elf        |  10 |  13 |  15 |  13 |  10 |  16 |
 | Halfling   |   9 |  13 |  15 |  15 |  13 |  16 |
 | Gnome      |  10 |  13 |  14 |  14 |  13 |  13 |
 | Dwarf      |  13 |   8 |  15 |  10 |  14 |  12 |
 +------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
 
4 Bard
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHA
 +------------+-----+-----+-----+-----+-----+-----+
 | Human      |  14 |  13 |  11 |  14 |  12 |  15 |
 | Half-Elf   |  13 |  14 |  11 |  15 |  11 |  16 |
 | Elf        |  13 |  15 |  12 |  15 |  10 |  16 |
 | Halfling   |  12 |  15 |  12 |  17 |  13 |  16 |
 | Phraint    |  14 |  13 |   7 |18/20|  12 |  12 |
 | Dryad      |  13 |  13 |  14 |  14 |  10 |  18 |
 +------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
 
4 Adventurer
.lm 1
.literal
                STR   INT   WIS   DEX   CON   CHA
 +------------+-----+-----+-----+-----+-----+-----+
 | Human      |  14 |  14 |  11 |  13 |  13 |  12 |
 | Half-Elf   |  13 |  15 |  11 |  14 |  12 |  13 |
 | Elf        |  13 |  16 |  12 |  14 |  11 |  13 |
 | Halfling   |  12 |  16 |  12 |  18 |  14 |  13 |
 | Phraint    |  14 |  14 |   7 |18/10|  13 |   9 |
 | Dryad      |  13 |  14 |  14 |  13 |  11 |  15 |
 +------------+-----+-----+-----+-----+-----+-----+
.end literal
.lm 0 .blank 2
1 Commands
.lm 1 .p
      All commands are  entered  by  pressing  a  single  key,  or
 control  sequence  (holding down the control key while pressing a
 key).  If a particular command  requires  additional  action,  it
 will  be  prompted  for, with one exception.  When a direction is
 required, no prompt is given unless an illegal response
 is given.
.lm 0 .blank 2
2 Directions
.lm 1 .p
      In the following instructions, the symbol <Dir> refers to  a
 numeric  direction  based on your keypad.  It is not valid to use
 the number '5' in this context.  One exception to  this  is  with
 movement, in which case '5' will rest the character for one turn.
.p
      Commands which require a direction will not prompt  you  for
 one,  unless  you  input  an  illegal  direction.   Just  enter a
 direction after the entering the command.
.literal


   Directions    \     |     /
                  7    8    9
                - 4         6 -
                  1    2    3
                 /     |     \
.end literal
.lm 0 .blank 2
2 Movement
.lm 1 .p
      Movement is  accomplished  by  using  your  numeric  keypad
 (or the number keys.)
 Simply  press  a  number and your character will move one step in
 that direction.  Pressing a '5' is equivalent to waiting for  one
 round  (More  efficient  resting  over  long  periods  of time is
 accomplished by using the Rest command).  You can only move  onto
 and through floor spots, and only if they contain no creatures or
 obstructing objects such as a closed door.
.p
      Moving your character  one  step  at  a  time  can  be  time
 consuming  and  boring, so a faster method has been supplied.  By
 using the Move command '.', you may move  in  a  direction  until
 something  interesting  happens.   For  instance, by pressing the
 period key '.' followed by the direction 8, your character  would
 continue  to  move  up the screen, only coming to a stop after at
 least one condition is satisfied.  These conditions are:
.blank 2
.lm 10
.list 0 'o'
.le;    A creature appears upon  the screen,  or a creature already
       on the screen moves.
.le;   You move next to an  object,  or feature  such as  a  door,
       staircase, or trap.
.le;    You come to  a  wall,  and  have more  than one  choice  of
       directions  from  which  to continue,  or are in a dead end
       passage.
.le;    You come to a junction of several passages.
.els
.literal
 
 Movement    \     |     /
              7    8    9
            - 4         6 -    5 rests the character for one turn.
              1    2    3
             /     |     \
.end literal
.lm 0 .blank 2
2 Age
.lm 1 .p
   A       - Shows the current age of the character.
.p
      The A command will print the current age of the character in
 years, days, hours, and minutes.
.lm 0 .blank 2
2 Bash
.lm 1
   B <Dir> - Bash
.p
      The Bash command includes breaking open doors and chests, or
 bashing an opponent.  Two main factors determine the ability of a
 character  to  bash -  their  weight  and  their  strength.    In
 addition,  when  bashing  an  opponent, you will either perform a
 body bash, or if wielding a shield, perform a shield  bash  which
 is more effective.
.p
      Bashing a door can throw the character off-balance, but this
 will  not  generally  be  a problem.  Doors that have been jammed
 closed with spikes can only be opened by bashing.   Locked  doors
 may  also  be  bashed  open.   Note that bashing a door open will
 permanently break it.
.p
      Bashing a creature has effects on both the  player  and  his
 opponent.   Depending  on  a character's dexterity, he may or may
 not be thrown off-balance allowing free moves to  his  opponents.
 If   the   bash  is  successful,  the  opponent  will  be  thrown
 off-balance for 1 to 3 turns, thus allowing  the  character  free
 hits or a chance to run.
.p
      A player automatically performs a shield bash instead  of  a
 body  bash, *if* he is currently wearing a shield.  A shield bash
 adds the damage of a shield to that of  the bash,  so it is  more
 effective.   Note  that  size and material both affect the damage
 that a shield will do.
.lm 0 .blank 2
2 Character
.lm 1
   C       - Print character (to screen or file)
.p
      This  command  allows  the  player  to  either  display  his
 character on the terminal screen, or to print an entire character
 info listing to a file.  If it is printed to a file, the history,
 equipment list, and inventory list are included.
.lm 0 .blank 2
2 Disarm
.lm 1
   D <Dir> - Disarm a trap.
.p
      You can attempt to disarm floor traps or trapped chests.  If
 you fail to disarm a trap, there is a chance that you blunder and
 set it off.  You can only disarm a trap on  a chest after finding
 it with the search command.
.lm 0 .blank 2
2 Eat
.lm 1
   E       - Eat some food.
.p
      A character must eat occasionally to remain effective.  As a
 character  grows  hungry,  a message will appear at the bottom of
 the screen saying "Hungry".  If a character remains  hungry  long
 enough, he will  become "Weak"  and  lose  weight  (reducing  his
 bashing effectiveness).  Continued  starvation will result in the
 character  "Fainting"  (passing  out  for  several  rounds).  The
 fainting  will  become  increasingly  common  until the character
 dies of starvation.
      Eating too much, however, can cause death from gluttony.
.p
      Some items increase or decrease the rate at which you digest
 food.
.lm 0 .blank 2
2 Fill
.lm 1
   F       - Fill a lamp or lantern with oil.
.p
      If your character is currently using a lamp for  light,  and
 if  he has a flask of oil in his inventory, he may refill the lamp by
 using  this  command.  A lamp  is  capable of a  maximum of 15000
 turns of light, and each flask has 7500 turns of oil contained in
 it.
.lm 0 .blank 2
.lt
2 Game_time
.el
.lm 1
   G       - Shows current game time.
.p
      This command will show the current game time. Since the time
 on the display is only updated every 15 minutes and does not have
 the year and month, this command will show the full time.
.lm 0 .blank 2
2 Help
.lm 1
   H       - Enter the Moria help library.
.p
      If you're here, you  already know how to get  into the Moria
 help  library.  You  may  return  to your  game by  entering a ^Z
 (<CONTROL>-Z) character at any prompt.

.lm 0 .blank 2
2 Inventory-Selective
.lm 1
   I       - Select and display an inventory of a class of items.
.p
      The I command allows you to choose what types of items you would
 like displayed.  Select the character that is used to display that type
 of item.  | for swords, ! for potions , etc.  From this list you can use
 three commands that are specific to the inventory lists.  See the entry
 title Inventory for more information.
.lm 0 .blank 2

2 Location
.lm 1
   L       - Display map co-ordinates.
.p
      The Location command will display your  character's  current
 co-ordinates  as  shown  on  a  printed map (printed with the 'P'
 command).  Sectors contain up to 44 rows by 99 columns each.  The
 Location  command  will  display  the character's current row and
 column map co-ordinates, as well as the sector number.
.lm 0 .blank 2
2 Money
.lm 1
   M       - Display current inventory of currency.
.p
      The M command will display the  inventory of  your  Mithril,
 Platinum, Gold, Silver, Copper, and Iron.
.lm 0 .blank 2
2 Print
.lm 1
   P       - Print map to file.
.p
      The Print command will write an entire map  of  the  dungeon
 floor  explored to a file.  Since the dungeon floor is large, the
 map is broken up into sectors, each containing up to 44  rows  by
 99 columns.
.lm 0 .blank 2
2 Rest
.lm 1
   R       - Rest for a number of turns.
.p
      You may rest one turn by pressing the '5' key.  Resting  for
 longer periods of time is accomplished by using the Rest command,
 followed by the number of turns you want to rest your  character.
 Resting  will  continue until the specified duration has expired,
 or something to wake the character happens, such  as  a  creature
 wandering  by.   It  is  sometimes  a good idea to rest a beat-up
 character until he regains some of his hit points, but be sure to
 have plenty of food if you rest often.
.p
      If you have accidently entered in a rest period  too  large,
 or  change  your mind about the resting period, you may wake your
 character up by pressing any key.
.lm 0 .blank 2
2 Search_Mode
.lm 1
   S       - Search mode toggle.
.p
      The Searching toggle will take the character into and out of
 searching mode.  When first pressed, the message "Searching" will
 appear at the bottom of the screen.  The character is now  taking
 two  turns  for each command, one for the command and one turn to
 search about him.  Note that this means he is  taking  twice  the
 time to move about the dungeon, and therefore twice the food.  If
 a creature should happen by  or  attack  you,  search  mode  will
 automatically  shut  off.  Otherwise you may turn off search mode
 by again pressing the 'S' key.
.lm 0 .blank 2
2 Tunnel
.lm 1
   T <Dir> - Tunnel through rock.
.p
      Tunneling (Mining) is a very useful  art.   There  are  four
 kinds  of rock present in the dungeons of Moria;  Permanent Rock,
 Granite Rock, Magma Intrusions, and Quartz Veins.  Permanent Rock
 is exactly that, permanent.  Granite is very hard, therefore hard
 to dig through, and  contains  no  valuable  metals.   Magma  and
 Quartz  veins  are  softer and sometimes bare valuable metals and
 gems, shown as a '$' or a '*' character.  You  can  tell  if  the
 metal  or  gems are  embedded in the wall by  trying to move onto
 them.  If you can't move over them, you'll have to dig them out.
.p
      Tunneling can be VERY difficult by hand, so when you dig  be
 sure  to  wield  either  a shovel or a pick.  Magical shovels and
 picks can be found which allow the wielder  to  dig  much  faster
 than normal, and a good strength also helps.
.lm 0 .blank 2
.lt
2 Using_Misc_Items
.el
.lm 1
   U       - Use a miscellaneous item.
.p
      Some miscellaneous items have special properties that can be
 invoked by trying to use them.
.lm 0 .blank 2
.lt
2 Show_time
.el
.lm 1
   W       - Displays the current time.
.p
      This is useful if you don't have a watch  and don't  want to
 spawn out just to show the time.
.lm 0 .blank 2
2 Aim
.lm 1
   a <Dir> - Aim a wand.
.p
      Wands must be aimed in a direction to be used.  Wands are  a
 magical  device  and therefore use the Magical Devices ability of
 the player.  They will either affect  the  first  object/creature
 encountered,  or  affect anything in a given direction, depending
 upon the wand.  An obstruction such as door or wall will  usually
 stop the effects of a wand from traveling further.
.lm 0 .blank 2
2 Browse
.lm 1
   b       - Browse a book.
.p
      You  can  only  read  a  book  if  you  are  of  its  realm.
 Therefore  a  magic-user  could read a magic book, but not a holy
 book.  Fighters will  not be able to read either  kind  of  book.
 When  the  browse  command  is used, all of the spells or prayers
 contained in the book along with information about  it,  such  as
 its level, the  amount of mana used up in casting it, and whether
 or not you know the spell or prayer will be displayed.
.lm 0 .blank 2
2 Close
.lm 1
   c <Dir> - Close a door.
.p
      Non-intelligent and certain other creatures will not be able
 to  open  a  door.  Therefore shutting doors can be a life saver.
 You must be adjacent to an open door, and you cannot close broken
 doors.  Note that bashing a door open will break it.
.lm 0 .blank 2
2 Drop
.lm 1
   d       - Drop an object or money from you inventory.
.p
      You can drop a single object or a pile of one coin type onto
 the floor beneath you if that floor spot does not already contain
 an object.  Note that doors and traps are considered  objects  in
 this  sense.  If you drop an object such as a potion or scroll, a
 single one is dropped onto the floor at a  time.   Group  objects
 such  as  arrows are all dropped at once onto the floor.  To drop
 money instead of an item, type $ when prompted for what  item  to
 drop,  and  you  will  be prompted for what type of money and how
 many of that coin you wish to drop.
.lm 0 .blank 2
2 Equipment
.lm 1
   e       - Display a list of equipment being used.
.p
      Use the Equipment command  to  display  a  list  of  objects
 currently  being  used  by your character.  Note that each object
 has a specific place were it is placed, and that only one  object
 of  each  type  may be used at any one time;  excepting rings, of
 which two can be worn, one on each hand.  There are three commands
 you can use from the equipment list that are specific to the inventory
 and equipment lists.  See the entry titled Inventory for further details.
.lm 0 .blank 2
2 Fire
.lm 1
   f       - Fire an arrow or bolt from bow/crossbow.
.p
Use the Fire command to shoot an arrow or bolt from a bow or crossbow
respectively.  Use of this command does far more damage than simply
(h)urling an item.
.lm 0 .blank 2
2 Hurl
.lm 1
   h       - Hurl any item.
.p
Use the hurl command to throw any item in any direction.  Some items 
cause damage when they hit a monster, others simply annoyt them.
2 Inventory
.lm 1
   i        - Switch to the inventory list.
.p
      The Inventory command displays a list of all  objects  being
 carried,  but  are not in current use.  You may carry an infinite
 amount of items, limited only  by  the  maximum  weight  you  can
 carry.  Initially you will be prompted for a 'B' to browse through
 your inventory until you find the correct page. Once there you can use
 a number of commands.  Some commands are specific to the inventory and
 equipment lists.  These commands are (p)ut, (r)emove and (c)lean.

3 Clean
.lm 1
   c       - Switches to inventory and drops and destroys selected items.
.p
     The c command allows you to page through your inventory and
 drop all of the unneccesary items you have accumulated.  Beware,
 all the items you drop cannot be recovered.
.lm 0 .blank 2
3 Put
.lm 1
   p       - Switch to inventory list and then put an item into another.
.p
     The p command allows you to place items into any bags of holding
 that you have in your inventory.  The items in bags of holding are not
 counted toward your weight.
.lm 0 .blank 2
3 Remove
.lm 1
   r       - Switch to inventory list and then remove an item from another.
.p
     The r command allows you to take items from any bags of holding
 that you have in your inventory.  The items taken from the bags will
 be counted toward your total weight.
.lm 0 .blank 2
2 Jam
.lm 1
   j <Dir> - Jam a door with an iron spike.
.p
      Most humanoid and many intelligent creatures can simply open
 a  closed  door,  and  can  eventually get through a locked door.
 Therefore you may spike a door in order to jam  it.   Each  spike
 used  on  a door will increase  its strength.  It is very easy to
 jam a door so much as to make it impossible for your character to
 bash  it  down,  so  spike  doors wisely.  Note that the bigger a
 creature is, the easier it can bash a door down.  Therefore  four
 or  more  spikes  might be necessary to slow down a dragon, where
 one spike would slow down a kobold.
.lm 0 .blank 2
2 Look
.lm 1
   l <Dir> - Look in a direction.
.p
      The Look command is useful in identifying the exact type  of
 object  or  creature shown on the screen.  Also, if a creature is
 on top of an object, the look command will  describe  both.   You
 can  see  creatures  and  objects up to 200 feet away (20 units).
 The command will  give you information  on ALL objects  along the
 line that you  tell it to  look in.  This  command is "Free" - in
 other words, it doesn't let the monsters take a move afterwards.
.lm 0 .blank 2
2 Magic
.lm 1
   m       - Cast a magic spell.
.p
      First, a character must have learned the spell before he can
 cast it.  Next, when casting a spell, he must have the book  that
 contains the spell, so a book containing the spell must be in his
 inventory.  Each spell has a chance of failure  which  starts out
 fairly  large  but  decreases  as a character gains levels.  If a
 character  does  not  have  the  available mana, he increases his
 chance of failure, and gambles on losing a point of constitution.
 Note  that  since a character must read the spell from a book, he
 cannot be blind or confused when casting a spell, and there  must
 be some light present.
.lm 0 .blank 2
2 Music
.lm 1
   m       - Sing from a music book.
.p
      First, a character must have learned the song before he  can
 cast  it.   Next, when singing a song, he must have the book that
 contains the song, so a book containing the song must be  in  his
 inventory.   Each  song  has a chance of failure which starts out
 failry large but decreases as a character  gains  levels.   If  a
 character  does  not  have  the  available mana, he increases his
 chance of failure, and gambles on losing  a  point  of  charisma.
 Note  that  since  a character must read the song from a book, he
 cannot be blind or confused when singing a song, and  there  must
 be some light present.
.lm 0 .blank 2
2 Open
.lm 1
   o <Dir> - Open a door, chest, or lock.
.p
      To open an object such as a door or chest you must  use  the
 Open  command.   If  the  object is locked, the Open command will
 attempt to pick the lock, based on  your  ability  at  disarming.
 Note  that if an object is trapped and you open it, the trap will
 be set off.
.lm 0 .blank 2
2 Pray
.lm 1
   p       - Read a prayer.
.p
      First, a character must have learned a prayer before he  can
 read  it.   Next,  when  reading  a prayer, he must have the book
 containing the prayer in his inventory.  Each prayer has a chance
 of  failure  which  starts  out  fairly  large but decreases as a
 character gains  levels.   If  a  character  does  not  have  the
 available  mana,  he increases his chance of failure, and gambles
 on losing a point of constitution.  Note that since  a  character
 must  read the prayer from a book, he cannot be blind or confused
 and there must be some light present.
.lm 0 .blank 2
2 Play
.lm 1
   p       - Play an instrument.
.p
      First, a character must have learned a song  before  he  can
 play  it.  Next, when playing a song, he must have the instrument
 that he plays the song in his inventory.  Each song has a  chance
 of  failure  which  starts  out  fairly  large but decreases as a
 character gains  levels.   If  a  character  does  not  have  the
 available  mana,  he increases his chance of failure, and gambles
 on losing a point of charisma.  Note that since a character  does
 not  need  to  see  to be able to play an instrument, he can play
 while blind or in a dark area.   Also  note  that  the  character
 cannot play while afraid.
.lm 0 .blank 2
2 Quaff
.lm 1
   q       - Quaff a potion.
.p
      To drink a potion use the Quaff command.  A  potion  affects
 the  player  in  some  manner.   The effects of the potion may be
 immediately noticed, or they may be subtle and unnoticed.
.lm 0 .blank 2
2 Read
.lm 1
   r       - Read a scroll.
.p
      To read a scroll use the Read command.  A scroll  spell  has
 an  area  affect,  except in a few cases such as identify scrolls
 which act on other objects.  Note that two scrolls, the  identify
 scroll  and  the  recharge  scroll, have titles which can be read
 without setting them off, and by pressing <ESCAPE> can  be  saved
 for future use.
.lm 0 .blank 2
2 Search
.lm 1
   s       - Search general area one turn.
.p
      The Search command can be used to locate  hidden  traps  and
 secret doors about the player.  Note that more than a single turn
 of searching will be required in most cases.  You  should  always
 search  a  chest  before  trying  to  open  it  because  they are
 generally trapped.
.lm 0 .blank 2
2 Take-Off
.lm 1
   t       - Take off a piece of equipment.
.p
      Use the Take-Off command to remove an object from  use,  and
 return  it  to  your inventory.  Occasionally you will run into a
 cursed item which cannot be removed.   Cursed  items  are  always
 bad, and can only be taken off after removing the curse.
.lm 0 .blank 2
2 Use
.lm 1
   u       - Use a staff.
.p
      The Use command will activate a staff.  Like  scrolls,  most
 staffs  have  an  area affect.  Becasue staffs are generally more
 powerful than most other items,  they  are  also  harder  to  use
 correctly.
.lm 0 .blank 2
2 Version
.lm 1
   v       - Display current version of game.
.p
      The Version command displays the  credits  for  the  current
 version of Moria.
.lm 0 .blank 2
 
2 Exchange
.lm 1
   x       - Exchange primary and secondary weapons.
.p
      A secondary weapon is any weapon which may be needed  often.
 Instead  of  searching  through  your  inventory, you may use the
 exchange command to keep the weapon ready.  For instance, if  you
 wanted  to  use your bow most of the time, but needed a sword for
 close combat, you  could  wield  your  sword,  use  the  exchange
 command to make it the secondary weapon, then wield your bow.  If
 the sword was suddenly needed, simply use the exchange command to
 switch  between the  bow and  the sword.  This  command is  NOT a
 "Free" command - the monsters can attack you while you are in the
 process of changing weapons!
.lm 0 .blank 2
2 Extinguish
.lm 1
   X       - Extinguish light source (or relight it)
.p
      Turn your light source on or off.  Very handy when resting.
 Without this command you either burned torches/lamps all the time
 or had to take your light off, rest, then rewield it.
.lm 0 .blank 2
2 Identify
.lm 1
   /       - Identify a character shown on screen.
.p
      Use this command  to find out what a character displayed  on
 the  screen  stands for.  For instance, by pressing '/.', you can
 find out that the '.' stands for  a  floor spot. When  used  with
 a creature, the identify command will tell you only what class of
 creature  the  symbol  stands  for, not  the  specific  creature,
 therefore  use  the  Look  command  for this information.
      This is not to be confused with the Identify SPELL, which is
 used to determine magical plusses (and other things) of an item.
.lm 0 .blank 2
2 Commands
.lm 1
   ?       - Display a list of commands.
.p
      The ? command  displays a  quick reference  help page on the
 screen.
.lm 0 .blank 2
.lt
2 Weapon_list
.el
.lm 1
   |       - Enter the help library at the weapon list.
.p
      This is a good short cut to see the list of  weapon  weights
 and prices.
.lm 0 .blank 2
.lt
2 Armor_list
.el
.lm 1
   ]       - Enter the help library at the armor list.
.p
      This is a good short cut to see  the  list of armor  weights
 and prices.
.lm 0 .blank 2
2 Repeat
.lm 1
   ^M      - Repeat last message.
.p
      The <Control>-M (Carriage-Return or Enter key) command  will
 re-display  the  last  message printed on the message line at the
 top of your screen.
.lm 0 .blank 2
2 Redraw
.lm 1
   ^R      - Redraw the screen.
.p
      To redraw the entire screen, use the <Control>-R command.
.lm 0 .blank 2
2 Quit
.lm 1
   ^C      - Quit the game without saving.
.p
      To exit the game  without  saving  your  character  use  the
 <Control>-C  command.  The computer will prompt  you to make sure
 that you really do want to  quit, because once you  exit the game
 in this manner, your character is not recoverable.
.lm 0 .blank 2
2 Save
.lm 1
   ^Y      - Save your character and quit the game.
.p
      To save your game so that it can be restarted later, use the
 <Control>-Y  command.   The save file can be moved about at will,
 but do  not  edit  it.   To restore  your game,  type  GAME MORIA
 name-of-save-file.  Note that a  copy of a saved  character  will
 not work after that character has died.
.lm 0 .blank 2
2 Shell
.lm 1
   $       - Shell out of game.
.p
      Use the Shell command '$' to temporarily exit  the  game  to
 execute  DCL  commands.  You may re-enter the game by typing in a
 LOGOUT or EOJ command.
.lm 0 .blank 2
2 Up
.lm 1
   <       - Climb an up-staircase.
.p
      If you move onto an up-staircase you may use the '<' command
 to  go  up  one level.  There is always one staircase going up on
 every level except for the  town level (this doesn't mean it will
 be easy to find).  Going up a staircase will always take you to a
 new dungeon area except for the town  level,  which  remains  the
 same  for  the duration of your character.  Note that some of the
 staircases are steep staircases, and can  take  you  up  to  four
 levels upwards.
.lm 0 .blank 2
2 Down
.lm 1
   >       - Descend a down-staircase.
.p
      If you are on top of a down-staircase you may  use  the  '>'
 command  to  go  down  one  level.  There are always two or three
 staircases going down on each level, except the town level  which
 has  only  three,  two  of  which are normal, and one of which is
 steep.  Going down will always take you to a  new  dungeon  area.
 Note  that  some  of the staircases are steep staircases, and can
 take you up to four levels downwards.
.lm 0 .blank 2
2 Go
.lm 1
   . <Dir> - Move in direction.
.p
      The  Move  command  '.'  will  move  you  in  the  indicated
 direction   until   one  of  several  conditions  happen.   These
 conditions  include,  a  creature  appearing  on  the  screen,  a
 creature  already on the screen moving, an object or feature such
 as a door, a staircase, or a trap is adjacent to  the  character,
 character  comes  into a junction of passages, or character comes
 to a wall with no choice or more than one  choice  of  continuing
 directions.
.lm 0 .blank 2
1 Disclaimer
.lm 1
      Moria is intended for Public Domain, and may not be sold  or
 marketed  IN  ANY  FORM without the permision and written consent
 from the author Robert Alan Koeneke.  I retain all copyrights  to
 this  program,  in  either the original or modified forms, and no
 violation,  deletion,  or  change  of  the  copyright  notice  is
 allowed.   Futhermore,  I will have no liability or responsibilty
 to any user with respect to loss or  damage  caused  directly  or
 indirectly by this program.
.lm 0 .blank 2
1 Help
.lm 1
.literal
 You can get help on a topic by typing
 
         HELP topic subtopic subsubtopic...
 
 A topic can have one of the following forms:
 
         1. An alphanumeric string (e.g. a command name, option, etc.)
         2. The match-all or wildcard symbol (*)
 
         Examples:       HELP AIM
                         HELP TOWN BUILDING
                         HELP A*
 
 If a topic is abbreviated, the text is displayed for each topic that
 matches the abbreviation.
.end literal
 
.lm 0 .blank 2
1 Qualifiers
.lm 1
.p
      There are several qualifiers that can be  used  with  the  GAME
 MORIA command.  The can be used in any combination.
.lm 0 .blank 2
2 Filename
.lm 1 .p
      A character  saved  with  the SAVE  (CTRL-Z)  command  can be
 restored for further playing by using the command:
.literal
 
      GAME MORIA filename
 
.end literal
.lm 0 .blank 2
3 Example
.lm 1 .p
      If you saved a character, and called the save  file  FOO.BAR,
 then you can continue your game by typing:
.literal
 
      GAME MORIA FOO.BAR
 
.end literal
.lm 0 .blank 2
2 /DIFFICULTY
.lm 1 .p
      If you want to play a game of Moria, but the normal game is
 too easy (or perhaps too hard) you can set the difficulty of the
 game by using:
.literal
 
      GAME MORIA/DIFFICULTY=5
 
.end literal
 which would set the difficulty to very hard.  You  can  set  the
 difficulty from 1 to 5, with 1 being very easy, and 5 being very
 hard.  You can only set the difficulty of the game when you  are
 creating  a  character, so if you do not set one when you create
 your character, the difficulty of the game will be 3. Also, the
 amount and quality of the treasure is increased at higher difficulties.
.lm 0 .blank 2
2 /HEAR
.lm 1 .p
      If you want to be warned about monsters that are out of sight
 but making noise in the water within hearing range, use:
.literal
 
      GAME MORIA/HEAR
 
.end literal
 and you will get a message every round that  there  are  creatures
 out  of sight but within hearing.  The default is to ignore things
 in the water.  When used this option can become rather annoying, however
 it is safer.
.lm 0 .blank 2
2 /SCORE
.lm 1 .p
      If  you'd like to  see the high score  list  without  having
 to kill off a character to do it, just use the command:
.literal
 
      GAME MORIA/SCORE
 
.end literal
 which will give you the entire high score list.  If you only want
 to see the first few scores, just use the command:
.literal
 
      GAME MORIA/SCORE=10
 
.end literal
 which would give you the top ten scores.
.lm 0 .blank 2
2 /TOP
.lm 1 .p
      When your character dies, normally the first 20  high  scores
 are  printed,  unless  you made the high score list, in which case
 the entire high score list is shown.  If you want to see  more  or
 less than the default of 20 scores, use the command:
.literal
 
      GAME MORIA/TOP=50
 
.end literal
 which would give you the first 50 high scores.
.lm 0 .blank 2
2 /TRAP
.lm 1 .p
      If you want messages that are sent to  your  terminal  to  be
 trapped  and  displayed  on  the  message  line  at the top of the
 screen, use:
.literal
 
      GAME MORIA/TRAP
 
.end literal
 to tell moria to trap messages.  The default is  to  let  messages
 come  through  normally.   Note  that checking for new messages is
 only made once per move.
.lm 0 .blank 2
2 Example
.lm 1 .p
      If you are rolling up a new character, and want to  play  the
 most  difficult  game, and want to have any messages that are sent
 to you put on the message line, you would use:
.literal
 
      GAME MORIA/DIFFICULTY=5/TRAP
 
.el
 If you wanted to restore an old character that  is  saved  in  the
 file  GRUNT.SAV  and  want  to hear when there are monsters in the
 water, you would use:
.literal
 
      GAME MORIA/HEAR GRUNT.SAV
 
.el
 Note that the qualifiers can be abbreviated to  anything  that  is
 unambigious, so the previous examples could become:
.literal
 
      GAME MORIA/D=5/TR
      GAME MORIA/H GRUNT.SAV
 
.el
.lm 0 .blank 2
1 Summary
.lm 1 .p
      The game of Moria is a single player dungeon simulation. The
 object is to decend deep into the vaults of Moria to  slay the
 evil Balrog, ruler of Moria.  This is a special  improved version
 of the original Moria and includes many new  objects and monsters,
 as well as an enlarged Town level and a few new classes and races
 to pit against the dungeon's inhabitants.
.p
      Before beginning your first adventure, you should  read this
 document  carefully.   Moria  is  a  complicated  game,  and will
 require a dedicated player to win.
.p
     If you have not played Moria  before, you should try to  play
 the basic version first. Improved Moria is more  complicated than
 normal Moria and you should have some experience before you  try  this
 game. Of course, if the other version is  unavailable to you, all
 that you need to know is in  this Help Library,  just remember to
 proceed through the dungeon cautiously.
.lm 0 .blank 2
1 Symbols
.lm 1 .p
      Symbols  on  your  map  can  be  broken  down   into   three
 categories:  Features of the dungeon such as walls, floor, doors,
 water and traps; Objects which can be picked up such as treasure,
 weapons, magical devices, etc;  and Monsters which may or may not
 move  about  the  dungeon,  but  are  mostly  harmful   to   your
 character's well-being.
.p
      Note that some symbols can be in  more  than  one  category.
 Also  note  that treasure may be embedded in a wall, and the wall
 must be removed before the treasure can be picked up.
.p
      It will not be necessary to remember all of the symbols  and
 their  meanings.   A simple command - the '/' - will identify any
 character appearing on your map.  See the section on commands for
 further help.
.lm 0 .blank 2
 
2 Features
.lm 1
.literal
 Features :
   .  A floor space, or hidden trap.
      An open pit. (empty space)
   `  A pool of water.
   #  A wall.                  
   '  An open door.
   +  A closed door.
   ^  A trap.
   <  A staircase up.
   >  A staircase down.
   ;  A loose floor stone.
   :  A pile of rubble.
.el
.lm 0 .blank 2
 
2 Objects
.lm 1
.literal
 Objects  :
   !  A flask or potion.             ?  A scroll or spell book.
   "  An amulet.                     [  Hard armor.
   $  Money (Can be in wall).        \  A hafted weapon.
   &  A chest.                       ]  Misc. armor.
   (  Soft armor.                    _  A staff.
   )  A shield.                      {  Missile (arrow, bolt, pebble).
   *  Gems (Can be in wall).         |  Sword or dagger.
   -  A wand.                        }  Missile weapon (Bow, crossbow, sling).
   /  A pole-arm.                    ~  Misc.
   =  A ring.                        ,  Food (not always safe).
   s  A skeleton.                    %  A musical instrument or song book.
.el
.lm 0 .blank 2
2 Land-Monsters
.lm 1
.literal
 Land Monster Catagories:
   a  Amphibian.                     A  Giant Ant Lion.
   b  Giant Bat.                     B  Major Demon (Beware!!!)
   c  Small Insect.                  C  Gelatinous Cube.
   d  Dragon (Young or Mature).      D  Ancient Dragon (Danger!)
   e  Floating Eye.                  E  Elemental.
   f  Fish.                          F  Fly.
   g  Golem.                         G  Ghost.
   h  Harpy.                         H  Hobgoblin
   i  Icky-Thing.                    I  Invisible Stalker.
   j  Canine.                        J  Jelly.
   k  Kobold.                        K  Killer Beetle.
   l  Giant Lice.                    L  Lich.
   m  Mold.                          M  Mummy.
   n  Naga.                          N  Nymph.
   o  Orc or Ogre.                   O  Ooze.
   p  Human(oid).                    P  Giant Human(oid).
   q  Quasit.                        Q  Quythulg.
   r  Rodent.                        R  Reptile.
   s  Skeleton.                      S  Scorpion.
   t  Giant Tick.                    T  Troll.
   u                                 U  Umber Hulk.
   v  Vapor.                         V  Vampire.
   w  Worm or Worm Mass.             W  Wight or Wraith.
   x  Spider.                        X  Xorn.
   y  Yeek.                          Y  Yeti.
   z  Zombie.                        Z  Giant Zombie.
   ,  Mushroom Patch                ??  A Mimic of some sort.
.el
.lm 0 .blank 2
2 Water-Monsters
.lm 1
.literal
 Water Monsters:
   a  Amphibian                      A
   b                                 B
   c                                 C  Alligator or Crocodile.
   d                                 D  Dinosaur.
   e                                 E
   f  Fish.                          F
   g                                 G
   h                                 H  Hippo.
   i                                 I
   j                                 J
   k                                 K
   l  Leech.                         L
   m  Man-o-War                      M
   n                                 N
   o                                 O
   p                                 P
   q                                 Q
   r                                 R Remora.
   s                                 S Shark.
   t                                 T
   u                                 U
   v                                 V
   w  Kelp.                          W
   x                                 X
   y                                 Y
   z                                 Z
.el
.lm 0 .blank 2
2 Town-Inhabitants
.lm 1
.literal
 Town inhabitants and shops:
   a                                 A  Alchemist Shop.
   b                                 B  First National Bank of Moria.
                                     B  Bull or Cow.
   c  Peasant's chicken.             C  KC's Casino.
   d  Villager's dog.                D
   e                                 E
   f  Goldfish                       F
   g                                 G  General Store.
   h                                 H
   i                                 I  Inn.
   j                                 J  Jewelry Shop.
   k  Stray kitten.                  K
   l                                 L  Library.
   m                                 M  Magic Item Shop.
   n                                 N
   o                                 O
   p  Townsperson.                   P  Trading Post.
   q                                 Q
   r                                 R  Armory.
   s                                 S
   t                                 T  Temple.
   u                                 U  Music Shop.
   v                                 V
   w                                 W  Weaponsmith.
   x                                 X
   y                                 Y
   z                                 Z
                                     +  Variety of Buildings.
.el
.lm 0 .blank 2
1 Town
.lm 1 .p
      The town level is where you will begin your adventure.   The
 town  consists  of  many  buildings, each  with an entrance, some
 townspeople, several fountains, and a wall  which  surrounds  the
 town.  The first time you are in town it will be daytime, but you
 may return to find that darkness has  fallen.   (Note  that  some
 spells may act differently in the town level.)
.lm 0 .blank 2
2 Buildings
.lm 1 .p
      You may enter any of the  buildings, if they are  open,  and
 if  the  building  contains a shop, you may barter with the owner
 for items you can afford.  But be  warned  that  the  owners  can
 easily be insulted, and may even throw you out for a while if you
 insult them too often.  To enter a building, simply move onto the
 entrance represented by a letter.   Note  that  if  you  enter  a
 building that is not a shop, the  inhabitant  may not be happy to
 see you and all of the shops have set hours of business.
.p
      Once inside a store, his inventory will appear on the screen
 along  with  a set of options for your character.  You may browse
 the store's inventory if it takes more than one page to  display,
 and  you  may  sell  or purchase items in his inventory.  You can
 execute your inventory and equipment commands to see what you are
 carrying.  Not shown with the options are the wear, take-off, and
 exchange commands which will also work, but were excluded to keep
 the options simple.
.p
      Stores do not always have everything in stock.  As the  game
 progresses,  they  may  get new items so check from time to time.
 Also, if you sell them an item, it may get  sold  to  a  customer
 while  you  are  adventuring so don't always expect to be able to
 get back anything you have sold.
.p
      Store owners will not buy harmful or useless items.   If  an
 object is unidentified, they will pay you some base price for it.
 Once they have bought  it  they  will  immediately  identify  the
 object.   If  it  is  a  good  object,  they will add it to their
 inventory.  If it was a bad bargain, they simply throw  the  item
 away.  In  any case,  you  may receive some knowledge of the item
 if another is encountered.
.lm 0 .blank 2
.lt
3 Alchemy_Shop
.el
.lm 1 .p
      The Alchemy Shop deals in all manner of potions and scrolls.
.lm 0
3 Armory
.lm 1 .p
      The Armory is where the  town's  armor  is  fashioned.   All
 sorts of protective gear may be bought and sold here.
.lm 0 .blank 2
3 Bank
.lm 1 .p
      The Bank is where you keep money that is too heavy to  carry
 around with you.  Also in the bank is a handy money changer and
 an insurance shop.
.lm 0 .blank 2
.lt
4 Insurance_Shop
.el
.lm 1 
.p
      The Insurance Shop is where you buy insurance for your items
 and character, so that if the system should  crash,  or  if  your
 terminal quits on you, you can cash in your insurance and get the
 character back.  Note the insurance  will only work when you save
 the character after buying it.
.lm 0 .blank 2
.lt
4 Money_Changer
.el
.lm 1
.p
 The Money Changer is where you can go to change all that drab useless
iron into beautiful mithril.  Of course it will take a *ton* (literally)
of iron to make a mitril piece, and the storekeeper keeps the change so
be careful.
.lm 0 .blank 2
.lt
3 General_Store
.el
.lm 1 .p
      The General Store sells food, drink, some clothing, torches,
 lamps, oil, and spikes.  All  of these items, and  others, can be
 sold back to the General store for money.
.lm 0 .blank 2
3 Library
.lm 1 .p
      The library contains all spell and prayer books for the mage
 and priest classes.  It also contains a few scrolls that  may  be
 useful to the character.
.lm 0 .blank 2
.lt
3 Magic_User's_Shop
.el
.lm 1 .p
    The magic shop deals in all sorts of rings, wands, amulets, and
 staves.
.lm 0 .blank 2
.lt
3 Music_Shop
.el
.lm 1 .p
      The Music Shop carries music books and  instruments  for  the
 druid  and  bard  classes, and also has a few other musical items
 that any character class can use.
.lm 0 .blank 2
3 Temple
.lm 1 .p
      The Temple deals in healing and restoration potions, as well
 as  bless scrolls, word-of-recall scrolls, some approved priestly
 weapons, etc.
.lm 0 .blank 2
.lt
3 Weaponsmith's_Shop
.el
.lm 1 .p
      The Weaponsmith's Shop  is  where  the  town's  weapons  are
 fashioned.   Hand  and  missile weapons may be purchased and sold
 here, along with arrows, bolts, and shots.
.lm 0 .blank 2
3 Inn
.lm 1 .p
      The Inn is a place where you may rest for a time - regaining
 hit points and mana - while you are waiting for an object to show
 up in another store (or waiting for a  store owner to forget that
 he doesn't like you). Lodging for one day doesn't include breakfast
 for that you need to buy room and board.
.lm 0 .blank 2
.lt
3 Trading_Post
.el
.lm 1 .p
      This is the most complicated store  of them all.  This store
 will buy anything that isn't  cursed (though you must identify it
 first, if you don't know what it is), and put  it up for sale for
 other Moria  players  to bid on.  After a  day or so,  the person
 who placed the highest bid on the item can come and pick it up at
 the store, and the person who sold it can come and collect 75% of
 the amount of money that the other person  paid for it (the owner
 of the shop has to make *some*  profit).  However, if the highest
 bid on the  item is below the  "normal" price for  the item, then
 the storekeeper will throw the item out,  and pay the original owner
 a fraction of the bid. Therefore,  it would  not be
 profitable  to sell "trivial"  items, because people will not put
 significant  bids on them, and  you'll end up  losing the item.
.p
      Occasionally  when you try  to enter this  shop, you will be
 informed that  "The shopkeeper is helping someone else".  This is
 not the same as being thrown out of the shop - you are just being
 told  that the  shopkeeper is  busy  helping  another person, and
 cannot help  both of you at  the same time.   Therefore, all  you
 have to do to remedy this is to come back a few minutes later and
 try to go in again.  The other player should be done by then.
.lm 0 .blank 2
2 Supplies
.lm 1 .p
      Your character will begin his adventure with  some  supplies
 already  on  him.   Use  the  Inventory 'i' command to check what
 these supplies are.  It will be necessary to buy  other  supplies
 before  continuing into the dungeon, however, so be sure to enter
 each of the stores.
.lm 0 .blank 2
2 Townsmen
.lm 1
.p
      The town contains many different kinds  of people.  In order
 to keep your character alive long enough to enter the dungeons of
 Moria in  the first place, you  should know a little about these
 people.
.p
      Of course in any fantasy town there should be animals about.
 And the town level is no exception. Almost  all  of  the  animals
 are harmless, but if you see a Raging Bull you do best  to  avoid
 him.   Since you are a stranger to the town  most of the  animals
 will follow you around, but no one wants their pet hurt, so watch
 out.
.p
      Blubbering Idiots are the least interesting of the people on
 town level.  All they ever do is walk up to you and drool on you.
 A bit  demoralizing,  perhaps, but not  dangerous.  Mangy Looking
 Lepers just beg for money, so they're about the same thing.  Kids
 like to follow strangers around but some of them might play jokes
 or kick you. But all three of these are only annoying; they do no
 harm.
.p
      Filthy Street Urchins beg  for money too,  but they have the
 ability  to actually *take*  some of it from you.  Keep away from
 them unless  you want to lose some of your gold.  They're easy to
 kill if you want to  get rid of them, but really aren't worth the
 effort.
.p
      Singing, Happy Drunks aren't  dangerous.  They  tend to move
 somewhat  randomly around the  town, carrying the money left over
 from their latest purchase of  Fine Ale  (general store), with no
 intentions of harming anyone.
.p
      Squint  Eyed  Rogues are a  different story,  however.  They
 will come  right up to you  and attack you from  20 squares away,
 and then steal items out of your  backpack!  Watch out for them -
 many  a Mage has had to start over again due to having his  spell
 book stolen by a Squint Eyed Rogue.
.p
      Mean Looking  Mercenaries and  Battle Scarred  Veterans  are
 nice enough, if you stay far  away from them.  They tend to enjoy
 picking  fights for  no apparent reason, though, so watch out for
 them, since  they are the most  dangerous people you will find in
 the  town.  Some  of  them  are  capable  of  killing  low  level
 characters in a single blow.
.p
      Most of the towns people should be avoided  by  the  largest
 possible  distance  when  you wander from store to store.  Fights
 will break out though, so be prepared.  Since your character grew
 up  in  this  world  of  intrigue,  no  experience is awarded for
 killing on the town level and if you attack non-harmful villagers
 your reputation will go down.
.p
      Reputation is important on town level.  It will effect  your
 bartering if it drops and the  townspeople  will  think  you  are
 dangerous if it gets too low, so you should remember that you are
 one of the good guys.
.lm 0 .blank 2
1 VMS_Help
.lm 1 .p
 This program is supposed to resemble the VMS Help command.
.p
 Here is some of the help that vms help provides for itself:

.literal
  If you type one of the listed  topics  in  response  to  the  Topic?
  prompt,  HELP  displays  information  about that topic and a list of
  subtopics (if there are any).  If one or more subtopics exist,  HELP
  will prompt you for a subtopic.
       Topic? Commands
       Commands
         .
         .  (HELP message text and subtopics)
         .
       COMMANDS Subtopic?

  If you type a subtopic name, HELP displays  information  about  that
  subtopic:
       COMMANDS Subtopic? Disarm
       Disarm
         .
         .  (HELP message text and subtopics, if any)
         .
       COMMANDS Subtopic?

  If one or more sub-subtopics exist,  HELP  will  prompt  you  for  a
  sub-subtopic;  otherwise, as in the above example, the facility will
  prompt you for another subtopic  of  the  topic  you  are  currently
  inspecting.

  Typing a question mark will redisplay the HELP message  and  options
  at  your  current level.  Pressing the Return key will either move you
  back to the previous HELP level if you are in a subtopic level, or
  terminate  HELP if you are at the first level.  Pressing Ctrl/C
  terminates HELP at any level.
.el
