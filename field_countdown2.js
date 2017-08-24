(function($) {
  Drupal.behaviors.field_countdown2 = {
    attach: function(context, settings) {
      var i = 0;
      $.each( $('.countdown-timer', context), function( index, element ) {
        var countdown = $('.countdown-timer-countdown', $(this));
        var note = $('.countdown-timer-note', $(this));
        var settings = Drupal.settings.field_countdown2[countdown.attr('id')];
        var field_time = settings['countdown-settings-time'];

        ts = new Date(field_time * 1000);
        countdown.countdown({
          timestamp: ts,
          callback: function(days, hours, minutes, seconds) {
            var date_time_str = new Array();
            date_time_str['@days'] = Drupal.formatPlural(
                    days, '1 day', '@count days'
                    );
            date_time_str['@hours'] = Drupal.formatPlural(
                    hours, '1 hour', '@count hours'
                    );
            date_time_str['@minutes'] = Drupal.formatPlural(
                    minutes, '1 minute', '@count minutes'
                    );
            date_time_str['@seconds'] = Drupal.formatPlural(
                    seconds, '1 second', '@count seconds'
                    );
            var message = Drupal.t(
                    '@days, @hours, @minutes and @seconds left',
                    date_time_str
                    );
            note.html(message);
          }
        });
      });
    }
  };
})(jQuery);
