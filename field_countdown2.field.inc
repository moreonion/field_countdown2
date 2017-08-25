<?php

/**
 * @file
 * Field API functions based on the date module.
 */

module_load_include('inc', 'date', 'date.field');

/**
 * Implements hook_field_load().
 */
function field_countdown2_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  date_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
}

/**
 * Implements hook_field_settings_form().
 */
function field_countdown2_field_settings_form($field, $instance, $has_data) {
  $form = date_field_settings_form($field, $instance, $has_data);

  // Fixate tz_handling to the timezone of the site.
  $form['tz_handling']['#default_value'] = 'site';
  $form['tz_handling']['#access'] = FALSE;

  // Disable setting of enddate.
  $form['enddate_get']['#default_value'] = 0;
  $form['enddate_get']['#access'] = FALSE;
  $form['enddate_required']['#default_value'] = 0;
  $form['enddate_required']['#access'] = FALSE;

  return $form;
}

/**
 * Implements hook_field_is_empty().
 */
function field_countdown2_field_is_empty($item, $field) {
  date_field_is_empty($item, $field);
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function field_countdown2_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  if ($display['type'] === 'field_countdown2_default'
      || $display['type'] === 'field_countdown2_with_text_timer'
  ) {
    $element['font_size'] = array(
      '#title' => t('Font size of countdown'),
      '#type' => 'textfield',
      '#default_value' => $settings['font_size'],
      '#element_validate' =>
        array('element_validate_integer_positive'),
      '#required' => TRUE,
    );

    $element['font_size_note'] = array(
      '#title' => t('Font size of text'),
      '#type' => 'textfield',
      '#default_value' => $settings['font_size_note'],
      '#element_validate' =>
        array('element_validate_integer_positive'),
      '#required' => TRUE,
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function field_countdown2_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  if ($display['type'] === 'field_countdown2_default'
    || $display['type'] === 'field_countdown2_with_text_timer'
  ) {
    $summary = t(
      'Font size of countdown is @size px. Font size of text is @note px.',
      array('@size' => $settings['font_size'], '@note' => $settings['font_size_note'])
    );
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function field_countdown2_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $font_size = $display['settings']['font_size'];
  $font_size_note = $display['settings']['font_size_note'];

  switch ($display['type']) {
    case 'field_countdown2_default':
      foreach ($items as $delta => $item) {
        if ($item['visibility'] === '1') {
          $endstamp = $item['value'];

          $element[$delta] = field_countdown2_display_timer(
            check_plain($endstamp),
            $font_size,
            $font_size_note,
            FALSE
          );
        }
      }
      break;

    case 'field_countdown2_with_text_timer':
      foreach ($items as $delta => $item) {
        if ($item['visibility'] === '1') {
          $endstamp = $item['value'];

          $element[$delta] = field_countdown2_display_timer(
            check_plain($endstamp),
            $font_size,
            $font_size_note,
            TRUE
          );
        }
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function field_countdown2_field_formatter_info() {
  return array(
    'field_countdown2_default' => array(
      'label' => t('jQuery Countdown Timer without text timer'),
      'field types' => array('field_countdown2'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'settings' => array(
        'font_size' => '48',
        'font_size_note' => '22',
      ),
    ),
    'field_countdown2_with_text_timer' => array(
      'label' => t('jQuery Countdown Timer with text timer'),
      'field types' => array('field_countdown2'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'settings' => array(
        'font_size' => '48',
        'font_size_note' => '22',
      ),
    ),
  );
}

/**
 * Implements hook_field_info().
 */
function field_countdown2_field_info() {
  $granularity = array('year', 'month', 'day', 'hour', 'minute');
  $settings = array(
    'settings' => array(
      'todate' => '',
      'granularity' => drupal_map_assoc($granularity),
      'tz_handling' => 'site',
      'timezone_db' => 'UTC',
      'visibility' => 1,
    ),
    'instance_settings' => array(
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
    ),
    // Integrate with the Entity Metadata module.
    'property_type' => 'date',
    'property_callbacks' => array('date_entity_metadata_property_info_alter'),
  );

  $return = array(
    'field_countdown2' => array(
      'label' => t('Countdown Timer Field'),
      'description' => t('Countdown Timer Field.'),
      'default_widget' => 'date_widget',
      'default_formatter' => 'field_countdown2_default',
    ) + $settings,
  );

  return $return;
}
