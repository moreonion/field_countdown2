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

  // Exported field_base: 'field_countdown2_heading'.
  $field_bases['field_countdown2_heading'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_countdown2_heading',
    'global_block_settings' => 2,
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  return $field_bases;
}
