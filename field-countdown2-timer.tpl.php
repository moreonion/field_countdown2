<?php

/**
 * @file
 * Provides a template for the Countdown Timer Field.
 */
?>

<div class='countdown-timer'>
  <?php if ($heading): ?>
    <h3><?php print $heading; ?></h3>
  <?php endif; ?>
  <div id='<?php print $timer_id ?>'
   class='countdown-timer-countdown clock'>
  </div>
</div>
