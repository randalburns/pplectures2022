#!/usr/bin/perl
$, = " ";
$\ = "\n";

$lastid = -1;
$lastfriend1 = -1;
$lastfriend2 = -1;
$output = 0;

while (<>)
{
  @trio = split;

  if ( ($trio[0] == $lastid) && 
       ($trio[1] == $lastfriend1) &&
       ($trio[2] == $lastfriend2 ) )
  {
    if ($output == 0)
    { 
      print @trio;
      $output = 1;
    }
  }
  else
  {
    $output = 0;
    $lastid = $trio[0];
    $lastfriend1 = $trio[1];
    $lastfriend2 = $trio[2];
  }
}
