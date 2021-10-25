#!/usr/bin/perl

$, = " ";
$\ = "\n";

while (<>) 
{
  @friends = split( /\s+/ );
  $size = @friends;

  $id = $friends[0];

  for ( $i=1; $i<$size; $i++ )
  {
    for ( $j=$i+1; $j<$size; $j++)
    {
      if ( $id < $friends[$j] )
      {
        print $friends[$i], $id, $friends[$j];
      }
      else
      {
        print $friends[$i], $friends[$j], $id;
      }

      if ( $id < $friends[$i] )
      {
        print $friends[$j], $id, $friends[$i];
      }
      else
      {
        print $friends[$j], $friends[$i], $id;
      }
    } 
  }
}
