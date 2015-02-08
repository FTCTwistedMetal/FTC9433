#Twisted Metal - 9433 | Cascade Effect
Cascade Effect 2014-2015
<br>
Uploaded code onto GitHub for efficiency and cleanness...
<br>
If you've stumbled upon this, feel free to browse and hopefully find some inspiration, but
please do not copy and paste anything. You won't learn and it's poor sportsmanship!
<br>
If you're unfamiliar with GitHub, you can check codes' progress by clicking on the code you wish to see,
then click "history," and click any version you want to look at. Green highlights are additions, red are
deleted, and no highlights are the same. You can click "view" if you want to see just the code version
without all the confusing highlight stuff.
<br><br>
For team members! If you want to download any version of a file, just go to it (described above) and
click "raw," then right click the page, and save as "blahblahblah.c" (NOT .txt file!). For the library
files, save it as ".h"

#Autonomous
I've tried recording the stages of autonomous, and now it's on Git, the history is saved! Hurray!
<br><br>
Currently, we only have one autonomous that gets the 60cm rolling and brings it to the parking zone.
Fortunately, we've implemented PID, compromising for the builders letting the wheels bend on all the weight of the lift...
<br><br>
Next steps in autnomous:
<br>
<Blockquote>
  -Selection screen housing multiple strategies/misc
  <Blockquote>
    -Will include selecting wait times, Ex// setting a delay before beginning to avoid crashing and what-nots
    <br>
    -Can select <i>which</i> rolling goals to bring to the parking zone. Oh that'd be nice
  </Blockquote>
  -Main Autonomous
  <Blockquote>
    -Get 60cm goal (preferably also the 30cm goal)
    <br>
    -Bring to parking zone (close to the ramp, so not to run into alliance)
    <br>
    -Seek beacon, score in center goal, and knock down kickstand
    <br>
    *We might have to trade off getting the rolling goals for scoring in the center goal*
  </Blockquote>
  -Misc
  <Blockquote>
    -Use a sonar to work around defensive robots? That might be a little too much for now
    <br>
    -One for parking zone (if we want rolling goals, we'll have to do that first, unless we want to keep track of whick IR position we're at)
    <br>
    -Tune PID and find ways to improve it!
  </Blockquote>
</Blockquote>

#Teleop (Driving)
To-Be-Finished
