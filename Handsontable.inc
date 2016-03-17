<?php

/**
 * @file
 * Defines callbacks and resources needed for the 'handsontable' form element.
 */
class Handsontable {

  /**
   * Loads the required resources for displaying the Handsontable element.
   * @param $form_state
   */
  public static function addRequiredResources(&$form_state) {
    drupal_add_css(drupal_get_path('module', 'handsontable') . '/bower_components/handsontable/dist/handsontable.full.min.css', array(
      'group' => CSS_DEFAULT,
      'every_page' => TRUE
    ));
    drupal_add_js(drupal_get_path('module', 'handsontable') . '/bower_components/handsontable/dist/handsontable.full.min.js', array(
      'group' => CSS_DEFAULT,
      'every_page' => TRUE
    ));
  }

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
    self::addRequiredResources($form_state);

    // Config JS
    $element['#attached']['js'] = [
      drupal_get_path('module', 'handsontable') . '/js/handsontable_field.config.js'
    ];

    // Determine already set values

    $values = [];
    foreach (element_children($element) as $key) {
      $values[$key] = [];
      foreach (element_children($element[$key]) as $subKey) {
        if (isset($element[$key][$subKey]['#default_value'])) {
          $values[$key][$subKey] = $element[$key][$subKey]['#default_value'];
        }
        else {
          $values[$key][$subKey] = NULL;
        }
      }
    }

    // Discover columns

    // TODO: Add error handling, see for example tabspanel
    $children = array_intersect_key($element[0], array_flip(element_children($element[0])));

    $colHeaders = [];
    $columns = [];
    foreach ($children as $name => $child) {
      $colHeaders[] = $child['#title'];
      $columns[] = $name;
    }

    // Pass settings
    $element['#attached']['js'][] = [
      'data' => [
        'handsontable' => [
          'names' => [$element['#name']],
          'data' => [json_encode($values)],
          'colHeaders' => [$colHeaders],
          'columns' => [$columns],
          'startRows' => [$element['#rows']]
        ]
      ],
      'type' => 'setting',
    ];

    $element['addRow'] = self::createChangeCallback($element, $form);

    return $element;
  }

  /**
   * Creates an AJAX callback button that allows change number of handsontable rows.
   *
   * @param array $element
   *   The handsontable form element.
   * @param array $complete_form
   *   The completed form.
   *
   * @return \FormElement The "add" button.
   *
   */
  protected static function createChangeCallback(array &$element, array &$complete_form) {
    $add['#type'] = 'button';
    $add['#id'] = $add['#name'] = $element['#id'] . '-callback';
    $add['#attributes'] = array('style' => 'display:none');
    $add['#ajax'] = array(
      'params' => array(
        'target' => $element['#hash']
      ),
      'callback' => 'xml_form_elements_ajax_callback',
      'method' => 'settings'
    );
    return $add;
  }
}

/**
 * Implements theme_hook().
 *
 * @param array $variables
 *   An array of theming variables.
 * @return string
 */
function theme_handsontable($variables) {
  $element = $variables['element'];

  unset($element['#prefix']);
  unset($element['#suffix']);

  $output = '<div class="handsontable-container">';

  $output .= '<div id="' . $element['#id'] . '"></div>';
  $output .= '<div id="' . $element['#id'] . '-table" class="handsontable"></div>';

  $output .= $element['#children'];
  $output .= '</div>';

  return $output;

}