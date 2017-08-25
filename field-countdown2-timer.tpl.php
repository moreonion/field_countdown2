<?php

/**
 * @file
 * Provides a template for the Countdown Timer Field.
 */
?>

<div class='countdown-timer'>
  <div id='<?php print $timer_id ?>'
   class='countdown-timer-countdown'>
  </div>
  <?php if($text_timer): ?>
    <div id='<?php print $note_id; ?>'
     class='countdown-timer-note'>
    </div>
  <?php endif; ?>
</div>
