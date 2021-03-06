/**
 * @file
 * Behavior for field_countdown2.
 *
 * Initializes the jQuery Countdown Timer.
 */

(function ($) {
  Drupal.behaviors.field_countdown2 = {
    attach: function (context, settings) {
      var i = 0;
      $.each($('.countdown-timer', context), function (index, element) {
        var countdown = $('.countdown-timer-countdown', $(this));
        var settings = Drupal.settings.field_countdown2[countdown.attr('id')];
        var ts = new Date(settings['countdown-settings-time'] * 1000);
        var left = Math.floor((ts - (new Date())) / 1000);
        if (left < 0) {
          left = 0;
        }
        var clock = $('.countdown-timer-countdown', $(this)).FlipClock( left, {
          clockFace: 'DailyCounter',
          countdown: true,
          language: settings['countdown-settings-language'],
        });
      });
    }
  };
})(jQuery);
