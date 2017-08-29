<?php

/**
 * @file
 * Field API functions based on the date module.
 */

module_load_include('inc', 'date', 'date.field');
module_load_include('inc', 'date', 'date_elements');
module_load_include('module', 'date', 'date');

/**
 * Implements hook_field_load().
 */
function field_countdown2_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  date_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
}

/**
 * Implements hook_field_widget_settings_form().
 */
function field_countdown2_field_widget_settings_form($field, $instance) {
  module_load_include('inc', 'date', 'date_admin');
  return _date_field_widget_settings_form($field, $instance);
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
 * Implements hook_field_instance_settings_form().
 *
 * Wrapper functions for date administration, included only when processing
 * field settings.
 */
function field_countdown2_field_instance_settings_form($field, $instance) {
  module_load_include('inc', 'date', 'date_admin');
  return _date_field_instance_settings_form($field, $instance);
}

/**
 * Implements hook_form_FORM_ID_alter() for field_ui_field_edit_form().
 */
function field_countdown2_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {
  if ($form['#field']['type'] === 'field_countdown2') {
    $form['#field']['type'] = 'datestamp';
    date_form_field_ui_field_edit_form_alter($form, $form_state, $form_id);
    $form['#field']['type'] = 'field_countdown2';
  }
}

/**
 * Implements hook_field_widget_form().
 */
function field_countdown2_field_widget_form(&$form, &$form_state, &$field, $instance, $langcode, $items, $delta, $element) {
  $field['type'] = 'datestamp';
  $element = date_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  $field['type'] = 'field_countdown2';

  $default = isset($items[$delta]['visibility']) ?
    $items[$delta]['visibility'] : $field['settings']['visibility'];
  $element['visibility'] = array(
    '#type' => 'checkbox',
    '#title' => t('Visible'),
    '#default_value' => $default,
  );

  // Strip field_countdown2_ from the widget_type to get a valid
  // date module widget type.
  $element['#widget_type'] = substr($element['#widget_type'], 17);
  $element['#element_validate'] = array('field_countdown2_validate');

  return $element;
}

/**
 * Implements hook_field_widget_properties_alter().
 *
 * Alters the widget properties of a field instance before it gets displayed.
 * Used here to flag new entities so we can later tell if they need default
 * values.
 */
function field_countdown2_field_widget_properties_alter(&$widget, $context) {
  if (in_array($widget['type'], array('field_countdown2_date_select',
    'field_countdown2_date_popup',
    'field_countdown2_date_text',
  ))) {
    $orig_type = $widget['type'];
    $widget['type'] = substr($widget['type'], 17);
    date_field_widget_properties_alter($widget, $context);
    $widget['type'] = $orig_type;
  }
}

/**
 * Implements hook_field_widget_info().
 */
function field_countdown2_field_widget_info() {
  $info = date_field_widget_info();

  $info['date_text']['field types'] = array('field_countdown2');
  $info['field_countdown2_date_text'] = $info['date_text'];
  unset($info['date_text']);

  $info['date_select']['field types'] = array('field_countdown2');
  $info['field_countdown2_date_select'] = $info['date_select'];
  unset($info['date_select']);

  if (module_exists('date_popup')) {
    $info['date_popup']['field types'] = array('field_countdown2');
    $info['field_countdown2_date_popup'] = $info['date_popup'];
    unset($info['date_popup']);
  }

  return $info;
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
      'Font size of countdown is @size em. Font size of text is @note em.',
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
        'font_size' => '4',
        'font_size_note' => '1',
      ),
    ),
    'field_countdown2_with_text_timer' => array(
      'label' => t('jQuery Countdown Timer with text timer'),
      'field types' => array('field_countdown2'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'settings' => array(
        'font_size' => '4',
        'font_size_note' => '1',
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
      'visibility' => 0,
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
      'default_widget' => 'field_countdown2_widget',
      'default_formatter' => 'field_countdown2_default',
      'default_token_formatter' => 'date_plain',
    ) + $settings,
  );

  return $return;
}
