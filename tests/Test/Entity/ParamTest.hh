<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Param;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Test <param> tag class implementation.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class ParamTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test the correct workflow for getElement method.
     *
     * @return void
     */
    public function testGetElement() : void
    {
        $document = new DOMDocument();

        $values = Vector{
            new Value(Vector{'foo'}, $document),
            new Value(Vector{'bar'}, $document),
            new Value(Vector{'baz'}, $document)
        };

        $param = new Param($values, $document);

        $document->appendChild($param->getElement());

        $xml = preg_replace('/\n/', '', $document->saveXML());

        $expectedXML = '<?xml version="1.0"?><param><value><string>foo</string></value><value><string>bar</string></value><value><string>baz</string></value></param>';

        static::assertSame($expectedXML, $xml);
    }

    /**
     * Test the fromNode error.
     *
     * @return void
     */
    public function testFromNodeError() : void
    {
        $node = new SimpleXMLElement('<invalidTag/>');

        $this->expectException(InvalidNodeException::class);
        Param::fromNode($node, new DOMDocument());
    }

    /**
     * Test the fromNode method correct workflow.
     *
     * @return void
     */
    public function testFromNode() : void
    {
        $document = new DOMDocument();

        $node = new SimpleXMLElement('
            <param>
                <value>
                    <string>foo</string>
                </value>
            </param>
        ');

        $paramEntity = Param::fromNode($node, $document);

        $this->assertCount(1, $paramEntity->getValues());
    }
}
