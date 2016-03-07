<?php

/**
 * @file
 * Defines callbacks and resources needed for the 'handsontable' form element.
 */
class Handsontable {

  /**
   * Loads the required resources for displaying the Handsontable element.
   *
   */
  public static function addRequiredResources(&$form_state) {
    drupal_add_css(drupal_get_path('module','handsontable') . '/bower_components/handsontable/dist/handsontable.full.min.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    drupal_add_js(drupal_get_path('module','handsontable') . '/bower_components/handsontable/dist/handsontable.full.min.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  }

  /**
   * The '#process' callback for the datepicker form element.
   *
   * @param array $element
   *   The datepicker form element.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $form
   *   The complete Drupal form definition.
   *
   * @return array
   *   The datepicker form element.
   */
  public static function process(array $element, array &$form_state, array $form = NULL) {
    self::addRequiredResources($form_state);

    // Config JS
    $element['#attached']['js'] = [
      drupal_get_path('module','handsontable') . '/js/handsontable_field.config.js'
    ];
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
function theme_handsontable($variables) {
  $element = $variables['element'];

  unset($element['#prefix']);
  unset($element['#suffix']);

  // Determine what the default value for the form element is. This will be passed to the handsontable_field config.
  if (isset($element['#value']) && strlen($element['#value'])) {
    $default = $element['#value'];
  }
  else {
    $default = drupal_json_encode(null);
  }

  // Pass settings
  $element['#attached']['js'][] = [
    'data' => [
      'handsontable' => [
        'ids' => [$element['#id']],
        'data' => [$default],
        'colHeaders' => [array_values($element['#attributes'])],
        'startRows' => [$element['#rows']]
      ]
    ],
    'type' => 'setting',
  ];

  $output = '';
  _form_set_class($element, ['form_handsontable']);

  $output .= '<div class="handsontable-container">';
  $output .= '<a class="table-add-row" data-action="addRow" href="#addrow" alt="Add a row">Add row</a> ';
  $name = isset($element['#name']) ? $element['#name'] : $element['#id'];
  $value = isset($element['#value']) ? check_plain($element['#value']) : '';
  $output .= '<input type="hidden" name="' . $name . '" id="' . $element['#id'] . '"' . ' value="' . $value . '" />';
  $output .= '<div id="' . $element['#id'] . '-table" class="handsontable"></div>';
  $output .= '</div>';

  $element['#children'] = $output;
  $element['#theme'] = 'form_element';

  return drupal_render($element);

}
