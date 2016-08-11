<?hh // strict

namespace Ivyhjk\Xml;

use SimpleXMLElement;

/**
 * Cast given values.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class Caster
{
    /**
     * Cast a value.
     *
     * @param string $to Cast type name (string, float, etc)
     * @param mixed $value The value to be casted.
     *
     * @return mixed
     */
    public static function cast(string $to, SimpleXMLElement $value) : mixed
    {
        switch ($to) {
            case 'string':
                return (string) $value;
                break;
            case 'int':
                return (int) $value;
                break;
            case 'float':
                return (float) $value;
                break;
            case 'double':
                return (float) $value;
                break;
            default:
                throw new \Ivyhjk\Xml\Exception\XmlException('Unsuported cast type!.');
        }
    }
}
