<?hh // strict

namespace Ivyhjk\Xml\Contract;

/**
 * Supported value types.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Contract
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
enum ValueType : string {
    /**
     * String support type.
     *
     * @const string
     */
    STRING = 'string';

    /**
     * Integer support type.
     *
     * @const string
     */
    INTEGER = 'int';

    /**
     * Float support type.
     *
     * @const string
     */
    FLOAT = 'float';

    /**
     * Double support type.
     *
     * @const string
     */
    DOUBLE = 'double';

    /**
     * Structs support (objects).
     *
     * @const string
     */
    STRUCT = 'struct';
}
