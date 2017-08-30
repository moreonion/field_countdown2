<?php

/**
 * @file
 * field_countdown2.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function field_countdown2_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_countdown'.
  $field_bases['field_countdown'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_countdown',
    'global_block_settings' => 2,
    'indexes' => array(),
    'locked' => 0,
    'module' => 'field_countdown2',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'heading' => NULL,
      'timezone_db' => 'UTC',
      'todate' => '',
      'tz_handling' => 'site',
      'visibility' => 0,
    ),
    'translatable' => 0,
    'type' => 'field_countdown2',
  );

  return $field_bases;
}
