<?hh // strict

namespace Ivyhjk\Xml\Exception;

/**
 * Handle unsupported value types.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Exception
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class UnsupportedValueType extends XmlException
{
    /**
     * Generate the new exception fot unsupported value types.
     *
     * @param string $type The unsupported type name.
     *
     * @return void
     */
    public function __construct(string $type) : void
    {
        $message = sprintf('Unsupported value type: "%s"', $type);

        parent::__construct($message);
    }
}
