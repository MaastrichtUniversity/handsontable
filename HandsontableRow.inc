<?php

/**
 * @file
 * Defines callbacks and resources needed for the 'handsontable' form element.
 */
class HandsontableRow {

  /**
   * The '#process' callback for the handsontable_row form element.
   *
   * @param array $element
   *   The handsontable_row form element.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $form
   *   The complete Drupal form definition.
   *
   * @return array
   *   The handsontable_row form element.
   */
  public static function process(array $element, array &$form_state, array $form = NULL) {
    return $element;
  }
}

/**
 * Implements theme_hook().
 *
 * @param array $variables
 *   An array of theming variables.
 * @return string
 */
function theme_handsontable_row($variables) {
  $element = $variables['element'];

  unset($element['#prefix']);
  unset($element['#suffix']);

  $element['#children'] = "";
  $element['#theme'] = 'form_element';

  // We render an empty element here intentionally. This element is purely
  return drupal_render($element);

}