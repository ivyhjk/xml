<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Entity\Member;
use Ivyhjk\Xml\Entity\Struct;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Test the <struct> tag class wrapper correct workflow.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class StructTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test the correct workflow for getElement method.
     *
     * @return void
     */
    public function testGetElement() : void
    {
        $document = new DOMDocument();

        $members = Vector{
            new Member(
                'foo',
                new Value(
                    Vector{
                        'foo'
                    },
                    $document
                ),
                $document
            )
        };

        $struct = new Struct($members, $document);

        $document->appendChild($struct->getElement());

        $expectedXML = '<?xml version="1.0"?><struct><member><name>foo</name><value><string>foo</string></value></member></struct>';

        $xml = preg_replace('/\n/', '', $document->saveXML());

        $this->assertSame($expectedXML, $xml);
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
        Struct::fromNode($node, new DOMDocument());
    }

    /**
     * Test the fromNode method correct workflow when is sent a nested struct.
     *
     * @return void
     */
    public function testFromNodeNested() : void
    {
        $node = new SimpleXMLElement('
            <struct>
                <member>
                    <name>foo</name>
                    <value>
                        <string>bar</string>
                    </value>
                </member>
                <member>
                    <name>bar</name>
                    <value>
                        <string>baz</string>
                    </value>
                </member>
                <member>
                    <name>baz</name>
                    <value>
                        <struct>
                            <member>
                                <name>zzz</name>
                                <value>
                                    <string>xxx</string>
                                </value>
                            </member>
                            <member>
                                <name>ccc</name>
                                <value>
                                    <struct>
                                        <member>
                                            <name>vvv</name>
                                            <value>
                                                <struct>
                                                    <member>
                                                        <name>bbb</name>
                                                        <value>
                                                            <string>nnn</string>
                                                        </value>
                                                    </member>
                                                </struct>
                                            </value>
                                        </member>
                                    </struct>
                                </value>
                            </member>
                        </struct>
                    </value>
                </member>
            </struct>
        ');

        $structEntity = Struct::fromNode($node, new DOMDocument());

        $this->assertCount(3, $structEntity->getMembers());
    }
}
