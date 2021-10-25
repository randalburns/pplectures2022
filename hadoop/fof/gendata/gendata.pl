#!/usr/bin/perl
$,="";
$\=" ";


# This script generates test inputs for the FoF program.
#  It just generates numbers randomly for friend ids, i.e.
#   it does not perform sampling without replacement.
#  Thus, it does not prevent multiple identical entries in 
#   the same friend list.  Nor does it prevent spurious
#   friends of friends from arising.

# Parse arguments?
# Probably not worth customizing for a tool.

$numids = 1000;
$numfriendpairs = 50000;
$numfoftrios = 1000;

mkdir ( "fof.input" ) or die "Failed to make output directory";
chdir ( "fof.input" ) or die "Failed to chdir";

for ( $i=0; $i<$numids; $i++)
{
  $handle = "friends" . $i;
  $filename = ">" . $handle;
  open ( $handle, "$filename" ) or die "Failed to open file. $!'";
  print $handle ( $i );
  close ($handle);
}

srand;

$j = 0;  # RB for accounting remove

for ( $i=0; $i<$numfriendpairs; $i++ )
{
   $a = int rand ( $numids );
   $b = int rand ( $numids );

   $handle = "friends" . $a;
   $filename = ">>" . $handle;
   open ( $handle, "$filename" ) or die "Failed to open file. $!'";
   print $handle ( $b );
   close ($handle);

   $handle = "friends" . $b;
   $filename = ">>" . $handle;
   open ( $handle, "$filename" ) or die "Failed to open file. $!'";
   print $handle ( $a );
   close ($handle);

   # Check it this is a FoF thing.  Only every 3rd pair (to keep it independent).
   #  this logic is weird, but it works.  
   #  On every 3rd pairing, (0,3,6,...) either 
   #    make 3 indedendent friends or make a FoF trio
#   if (( $i % 3 == 0 ) && ((int rand(int($numfriendpairs/$numfoftrios/3))) == 0))
   if ( 0 )
   {
     $c = int rand ( $numids );
     $handle = "friends" . $c;
     $filename = ">>" . $handle;
     open ( $handle, "$filename" ) or die "Failed to open file. $!'";
     print $handle ( $a );
     print $handle ( $b );
     close ($handle);

     $handle = "friends" . $a;
     $filename = ">>" . $handle;
     open ( $handle, "$filename" ) or die "Failed to open file. $!'";
     print $handle ( $c );
     close ($handle);

     $handle = "friends" . $b;
     $filename = ">>" . $handle;
     open ( $handle, "$filename" ) or die "Failed to open file. $!'";
     print $handle ( $c );
     close ($handle);

     $j++;
     $i += 2;
     print "Generated trio $a $b $c.  Iteration $i\n";
   }
   print "Iteration $i\n";
}

# Let's eliminate duplicates
for ( $i=0; $i<$numids; $i++)
{
  $handle = "friends" . $i;
  $filename = "+<" . $handle;
  open ( $handle, "$filename" ) or die "Failed to open file. $!'";
  $line = <$handle>;

  @friends = split ( /\s+/, $line );
  $size = @friends;
  for ( $j=0; $j<$size; $j++ )
  {
    for ( $k=$j+1; $k<$size; $k++ )
    {
      if ($friends[$j] == $friends[$k])
      {
        $friends[$k] = -1;
      }
    }
  }

  truncate  ($handle, 0);
  seek $handle, 0, 0;
  for ($j=0; $j<$size; $j++)
  {
    if ( $friends[$j] != -1 )
    {
      print $handle ( $friends[$j] )
    }
  }
  close ($handle);
}
